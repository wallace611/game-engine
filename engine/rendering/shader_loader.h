#pragma once

#include <glad/glad.h>
#include <GL/glut.h>

#include <string>
#include <unordered_map>

static std::unordered_map<std::string, GLuint> shaderMapper;

GLuint GetShader(const std::string& file_path);
GLuint LoadShader(const std::string& file_path);