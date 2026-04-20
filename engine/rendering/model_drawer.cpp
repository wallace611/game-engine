#include "model_drawer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "shader_loader.h"
#include "utilities/stb_image.h"
#include "utilities/happly.h"

#include <iostream>

void ModelDrawer::LoadModel(const std::string &model_path) {
    happly::PLYData plyIn(model_path);
    happly::Element& vertexElement = plyIn.getElement("vertex");
    
    std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
    size_t vertexCount = vPos.size();

    // Check if the file contains normal vectors
    bool hasNormal = vertexElement.hasProperty("nx") &&
        vertexElement.hasProperty("ny") &&
        vertexElement.hasProperty("nz");

    bool hasColor = vertexElement.hasProperty("red") &&
        vertexElement.hasProperty("green") &&
        vertexElement.hasProperty("blue") &&
        vertexElement.hasProperty("alpha");
    
    
    bool hasTexture = vertexElement.hasProperty("s") && vertexElement.hasProperty("t");

    std::vector<double> vNx, vNy, vNz, vTexS, vTexT;
    std::vector<unsigned char> vRed, vGreen, vBlue, vAlpha;

    if (hasNormal) {
        vNx = vertexElement.getProperty<double>("nx");
        vNy = vertexElement.getProperty<double>("ny");
        vNz = vertexElement.getProperty<double>("nz");
    }

    if (hasColor) {
        vRed = vertexElement.getProperty<unsigned char>("red");
        vGreen = vertexElement.getProperty<unsigned char>("green");
        vBlue = vertexElement.getProperty<unsigned char>("blue");
        vAlpha = vertexElement.getProperty<unsigned char>("alpha");
    }

    if (hasTexture) {
        vTexS = vertexElement.getProperty<double>("s");
        vTexT = vertexElement.getProperty<double>("t");
    }

    std::vector<float> vertices;
    for (size_t i = 0; i < vertexCount; i++) {
        // Add position
        vertices.push_back(static_cast<float>(vPos[i][0]));
        vertices.push_back(static_cast<float>(vPos[i][1]));
        vertices.push_back(static_cast<float>(vPos[i][2]));

        // Add normal
        if (hasNormal) {
            vertices.push_back(static_cast<float>(vNx[i]));
            vertices.push_back(static_cast<float>(vNy[i]));
            vertices.push_back(static_cast<float>(vNz[i]));
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
        }

        // Add color
        if (hasColor) {
            vertices.push_back(static_cast<float>(vRed[i]) / 256.0f);
            vertices.push_back(static_cast<float>(vGreen[i]) / 256.0f);
            vertices.push_back(static_cast<float>(vBlue[i]) / 256.0f);
        }
        else {
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
        }

        // Add texture coord
        if (hasTexture) {
            vertices.push_back(static_cast<float>(vTexS[i]));
            vertices.push_back(static_cast<float>(vTexT[i]));
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();
    std::vector<unsigned int> indices;

    for (const auto& face : fInd) {
        if (face.size() >= 3) {
            indices.push_back(static_cast<unsigned int>(face[0]));
            indices.push_back(static_cast<unsigned int>(face[1]));
            indices.push_back(static_cast<unsigned int>(face[2]));

            if (face.size() == 4) {
                indices.push_back(static_cast<unsigned int>(face[0]));
                indices.push_back(static_cast<unsigned int>(face[2]));
                indices.push_back(static_cast<unsigned int>(face[3]));
            }
        }
    }

    indicesCount = indices.size();
    size_t stride = 11 * sizeof(float);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*) (9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelDrawer::LoadTexture(const std::string &texture_path) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    
    if (data) {
        // Determine the image format based on channel count
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        
        // Upload the image data to the GPU memory
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        // Automatically generate mipmaps for better distant rendering quality
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << texture_path << std::endl;
    }

    // Free the raw image data from CPU RAM since it is now safely on the GPU
    stbi_image_free(data);
}

ModelDrawer::ModelDrawer() {}

ModelDrawer::ModelDrawer(const std::string &model_path,
                         const std::string &texture_path,
                         const std::string &shader_path) {

    program = LoadShader(shader_path);
    LoadModel(model_path);
    if (!texture_path.empty()) LoadTexture(texture_path);
    else texture = 0;
}

ModelDrawer::ModelDrawer(const std::string& model_path) : ModelDrawer(model_path, "") {}

ModelDrawer::ModelDrawer(const std::string &model_path, const std::string &texture_path) :
    ModelDrawer(model_path, texture_path, "shader/default_shader") {}

void ModelDrawer::Draw() {
    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "hasTexture"), texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}