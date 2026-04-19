#include "collider_drawer.h"

#include "rendering/shader_loader.h"

#include <glm/gtc/constants.hpp>

ColliderDrawer::ColliderDrawer(const glm::vec4 &color, float faceAlpha) : ModelDrawer() {
    this->color = color;
    this->faceAlpha = faceAlpha;
    this->program = LoadShader("shader/debug_shader");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int indices[] = {
        // --- Faces (Triangles): 36 indices (0 to 35) ---
        0, 1, 2, 2, 3, 0, // Back
        4, 5, 6, 6, 7, 4, // Front
        0, 3, 7, 7, 4, 0, // Left
        1, 5, 6, 6, 2, 1, // Right
        0, 1, 5, 5, 4, 0, // Bottom
        3, 2, 6, 6, 7, 3, // Top

        // --- Edges (Lines): 24 indices (36 to 59) ---
        0, 1, 1, 2, 2, 3, 3, 0, // Back loop
        4, 5, 5, 6, 6, 7, 7, 4, // Front loop
        0, 4, 1, 5, 2, 6, 3, 7  // Connecting edges
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    faceIndexCount = 36;
    lineIndexCount = 24;
    lineOffset = faceIndexCount;
}

void ColliderDrawer::UpdateAABB(const glm::vec3 &minPt,
                                const glm::vec3 &maxPt) {
    float vertices[] = {
        // Back face
        minPt.x, minPt.y, minPt.z, // 0
        maxPt.x, minPt.y, minPt.z, // 1
        maxPt.x, maxPt.y, minPt.z, // 2
        minPt.x, maxPt.y, minPt.z, // 3

        // Front face
        minPt.x, minPt.y, maxPt.z, // 4
        maxPt.x, minPt.y, maxPt.z, // 5
        maxPt.x, maxPt.y, maxPt.z, // 6
        minPt.x, maxPt.y, maxPt.z  // 7
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ColliderDrawer::UpdateSphere(const glm::vec3 &center, float radius,
                                  int sectors, int stacks) {
    std::vector<float> vertices;
    std::vector<unsigned int> faceIndices;
    std::vector<unsigned int> lineIndices;

    float x, y, z, xy;                              // vertex position
    float sectorStep = 2 * glm::pi<float>() / sectors;
    float stackStep = glm::pi<float>() / stacks;
    float sectorAngle, stackAngle;

    // Generate Vertices
    for (int i = 0; i <= stacks; ++i) {
        stackAngle = glm::pi<float>() / 2 - i * stackStep; // From pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        for (int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;           // From 0 to 2pi

            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            
            vertices.push_back(x + center.x);
            vertices.push_back(y + center.y);
            vertices.push_back(z + center.z);
        }
    }

    // Face indices
    int k1, k2;
    for (int i = 0; i < stacks; ++i) {
        k1 = i * (sectors + 1);     // beginning of current stack
        k2 = k1 + sectors + 1;      // beginning of next stack

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                faceIndices.push_back(k1);
                faceIndices.push_back(k2);
                faceIndices.push_back(k1 + 1);
            }
            if (i != (stacks - 1)) {
                faceIndices.push_back(k1 + 1);
                faceIndices.push_back(k2);
                faceIndices.push_back(k2 + 1);
            }
            
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0) {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // Update buffers
    faceIndexCount = faceIndices.size();
    lineIndexCount = lineIndices.size();
    lineOffset = faceIndexCount;

    std::vector<unsigned int> allIndices = faceIndices;
    allIndices.insert(allIndices.end(), lineIndices.begin(), lineIndices.end());

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int), allIndices.data(), GL_DYNAMIC_DRAW);
    
    glBindVertexArray(0);
}

void ColliderDrawer::Draw() {
    glUseProgram(program);
    glBindVertexArray(vao);

    GLint colorLoc = glGetUniformLocation(program, "debugColor");

    // Draw faces
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glUniform4f(colorLoc, color.r, color.g, color.b, faceAlpha * color.a);
    glDrawElements(GL_TRIANGLES, faceIndexCount, GL_UNSIGNED_INT, (void*)0);

    // Draw edges
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    glDrawElements(GL_LINES, lineIndexCount, GL_UNSIGNED_INT, (void*)(lineOffset * sizeof(unsigned int)));

    glBindVertexArray(0);
    glUseProgram(0);
}
