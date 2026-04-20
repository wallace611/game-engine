#include "phong_drawer.h"
#include <iostream>

#include "engine.h"

PhongDrawer::PhongDrawer(const std::string& model_path, Object* parent)
    : PhongDrawer(model_path, "", parent) {}

PhongDrawer::PhongDrawer(const std::string& model_path, const std::string& texture_path, Object* parent)
    : ModelDrawer(model_path, texture_path, "shader/phong_shader"),
      lightColor(1.0f, 1.0f, 1.0f),
      constant(1.0f), linear(0.027f), quadratic(0.0028f),
      useCustomColor(false), customColor(1.0f, 1.0f, 1.0f)
{
    this->parent = parent;
}

void PhongDrawer::Draw() {
    glUseProgram(program);

    // Pass light properties
    glUniform3fv(glGetUniformLocation(program, "lightPosWorld"), 1, glm::value_ptr(GetScene()->GetLight()->GetGlobalPosition()));
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(program, "viewPosWorld"), 1, glm::value_ptr(GetScene()->GetCamera()->GetGlobalPosition()));

    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(parent->GetGlobalMatrix()));

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