#include "shader_loader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

GLuint GetShader(const std::string& file_path) {
    if (shaderMapper.count(file_path)) {
        return shaderMapper[file_path];
    }
    return LoadShader(file_path);
}

GLuint LoadShader(const std::string& file_path) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertShaderFile;
    std::ifstream fragShaderFile;

    vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        vertShaderFile.open(file_path + ".vert");
        fragShaderFile.open(file_path + ".frag");
        std::stringstream vertShaderStream, fragShaderStream;
        vertShaderStream << vertShaderFile.rdbuf();
        fragShaderStream << fragShaderFile.rdbuf();

        vertShaderFile.close();
        fragShaderFile.close();
        vertexCode = vertShaderStream.str();
        fragmentCode = fragShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "Error occurred when reading file.\n";
    }

    GLuint vertex, fragment;
    int isSuccess;
    char infoLog[512];
    const char* cVertexCode = vertexCode.c_str();
    const char* cFragmentCode = fragmentCode.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &cVertexCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "Vertex shader compliation failed.\n" << infoLog << "\n";
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &cFragmentCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "Fragment shader compliation failed.\n" << infoLog << "\n";
    }

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed.\n" << infoLog << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return programID;   
}
