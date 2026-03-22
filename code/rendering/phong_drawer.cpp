#include "phong_drawer.h"

#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

PhongDrawer::PhongDrawer(const std::string &shader_path, const std::string &texture_path, Object *owner) 
: ModelDrawer(shader_path, texture_path) {
    this->owner = owner;
}

void PhongDrawer::Draw() {
    glUseProgram(program);

    glBindTexture(GL_TEXTURE_2D, texture);

    const glm::mat4& modelMatrix = owner->GetGlobalMatrix();
    glm::vec3 lightPos = glm::vec3(GetScene()->GetLightSource()->GetGlobalMatrix()[3]);
    glm::vec3 cameraPos = glm::vec3(GetScene()->GetCamera()->GetCameraPosition());

    GLint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

    GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));

    GLint lightColorLoc = glGetUniformLocation(program, "lightColor");
    // Using a nice warm sunlight color
    glUniform3f(lightColorLoc, 1.0f, 0.9f, 0.8f); 

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);

    glBindVertexArray(0);
    glUseProgram(0);
}