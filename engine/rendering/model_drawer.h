#pragma once

#include <glad/glad.h>
#include <GL/glut.h>

#include <string>

class ModelDrawer {
protected:
    GLuint vao, vbo;
    GLuint texture;
    GLuint program;
    unsigned int verticesCount;

    void SetupCube();
    void LoadTexture(const std::string& texture_path);

public:
    ModelDrawer(const std::string& shader_path, const std::string& texture_path);
    virtual void Draw();
};