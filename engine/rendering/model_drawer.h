#pragma once

#include <glad/glad.h>
#include <GL/glut.h>

#include <string>

class ModelDrawer {
protected:
    GLuint vao, vbo, ebo;
    unsigned int indicesCount;
    GLuint texture;
    GLuint program;
    unsigned int verticesCount;

    void LoadModel(const std::string& model_path);
    void LoadTexture(const std::string& texture_path);

    // For subclass
    ModelDrawer();
    ModelDrawer(const std::string& model_path, const std::string& texture_path, const std::string& shader_path);

public:
    ModelDrawer(const std::string& model_path);
    ModelDrawer(const std::string& model_path, const std::string& texture_path);
    virtual void Draw();
    inline GLuint GetTextureID() const { return texture; }
};