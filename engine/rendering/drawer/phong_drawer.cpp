#include "phong_drawer.h"
#include <iostream>

PhongDrawer::PhongDrawer(const std::string& model_path)
    : PhongDrawer(model_path, "") {}

PhongDrawer::PhongDrawer(const std::string& model_path, const std::string& texture_path)
    : ModelDrawer(model_path, texture_path, "shader/phong_shader"),
      lightPosWorld(0.0f, 0.0f, 0.0f), lightColor(1.0f, 1.0f, 1.0f),
      constant(1.0f), linear(0.09f), quadratic(0.032f),
      useCustomColor(false), customColor(1.0f, 1.0f, 1.0f) 
{
}

void PhongDrawer::Draw() {
    glUseProgram(program);

    // Pass light properties
    glUniform3fv(glGetUniformLocation(program, "lightPosWorld"), 1, glm::value_ptr(lightPosWorld));
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));

    // Pass attenuation parameters
    glUniform1f(glGetUniformLocation(program, "constant"), constant);
    glUniform1f(glGetUniformLocation(program, "linear"), linear);
    glUniform1f(glGetUniformLocation(program, "quadratic"), quadratic);

    // Pass custom color toggle and values
    glUniform1i(glGetUniformLocation(program, "useCustomColor"), useCustomColor ? 1 : 0);
    glUniform3fv(glGetUniformLocation(program, "customColor"), 1, glm::value_ptr(customColor));

    // Texture binding check
    bool hasTexture = (texture != 0);
    glUniform1i(glGetUniformLocation(program, "hasTexture"), hasTexture ? 1 : 0);

    if (hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(program, "texture_diffuse1"), 0);
    }

    // Execute drawing commands
    glBindVertexArray(vao);
    if (indicesCount > 0) {
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    }

    // Cleanup states
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PhongDrawer::SetLightWorldSpace(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::vec3& color) {
    // Transform the light's world position into view space for the shader
    glm::vec4 viewSpacePos = viewMatrix * glm::vec4(worldPos, 1.0f);
    lightPosWorld = glm::vec3(viewSpacePos);
    lightColor = color;
}

void PhongDrawer::SetAttenuation(float c, float l, float q) {
    constant = c;
    linear = l;
    quadratic = q;
}

void PhongDrawer::EnableCustomColor(bool enable) {
    useCustomColor = enable;
}

void PhongDrawer::SetCustomColor(const glm::vec3& color) {
    customColor = color;
}