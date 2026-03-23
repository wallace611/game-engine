#include "model_drawer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "shader_loader.h"
#include "utilities/stb_image.h"

#include <iostream>

void ModelDrawer::SetupCube() {
    // 36 vertices for a cube (6 faces * 2 triangles * 3 vertices)
    // Format: Position(XYZ), Normal(XYZ), Color(RGB), TexCoord(UV)
    // Total: 11 floats per vertex
    float vertices[] = {
        // Back face (Normal: 0, 0, -1)
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 
        
        // Front face (Normal: 0, 0, 1)
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 
        
        // Left face (Normal: -1, 0, 0)
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
        
        // Right face (Normal: 1, 0, 0)
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,         
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,     
        
        // Bottom face (Normal: 0, -1, 0)
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
        
        // Top face (Normal: 0, 1, 0)
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,     
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f  
    };

    verticesCount = 36;
    size_t stride = 11 * sizeof(float);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Location 0: Position (XYZ)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    // Location 1: Normal (XYZ)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Location 2: Color (RGB)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Location 3: TexCoord (UV)
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
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

ModelDrawer::ModelDrawer(const std::string& texture_path) : ModelDrawer("shader/shader", texture_path) {}

ModelDrawer::ModelDrawer(const std::string &shader_path, const std::string &texture_path) {
    program = LoadShader(shader_path);
    SetupCube();
    LoadTexture(texture_path);
}

void ModelDrawer::Draw() {
    glUseProgram(program);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);

    glBindVertexArray(0);
    glUseProgram(0);
}