#include "phong_drawer.h"
#include "engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object/light_object.h"

PhongDrawer::PhongDrawer(const std::string &texture_path, Object *owner) 
: ModelDrawer("shader/phong", texture_path) {
    this->owner = owner;
}

void PhongDrawer::SetSettings(const PhongSettings& newSettings) {
    settings = newSettings;
}

PhongSettings PhongDrawer::GetSettings() const {
    return settings;
}

void PhongDrawer::SetEnvironmentMap(GLuint textureID) {
    envTexture = textureID;
    hasEnvMap = true;
}

void PhongDrawer::Draw() {
    glUseProgram(program);

    // --- 1. Bind the base texture to Unit 0 ---
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);

    // --- 2. Bind the environment map to Unit 1 (If exists) ---
    if (hasEnvMap) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, envTexture);
        glUniform1i(glGetUniformLocation(program, "equirectangularMap"), 1);
    }

    const glm::mat4& modelMatrix = owner->GetGlobalMatrix();
    glm::vec3 lightPos = glm::vec3(GetScene()->GetLightSource()->GetGlobalMatrix()[3]);
    glm::vec3 cameraPos = glm::vec3(GetScene()->GetCamera()->GetLocalPosition());

    // --- Standard Matrices and Transforms ---
    GLint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

    GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));

    GLint lightColorLoc = glGetUniformLocation(program, "lightColor");
    glm::vec3 lightColor = dynamic_cast<LightObject*>(GetScene()->GetLightSource())->GetLightColor();
    glUniform3f(lightColorLoc, lightColor[0], lightColor[1], lightColor[2]); 

    // --- Apply Phong Settings to Shader Uniforms ---
    glUniform1f(glGetUniformLocation(program, "constant"), settings.constant);
    glUniform1f(glGetUniformLocation(program, "linear"), settings.linear);
    glUniform1f(glGetUniformLocation(program, "quadratic"), settings.quadratic);
    
    glUniform1f(glGetUniformLocation(program, "ambientStrength"), settings.ambientStrength);
    glUniform1f(glGetUniformLocation(program, "specularStrength"), settings.specularStrength);
    glUniform1f(glGetUniformLocation(program, "shininess"), settings.shininess);

    glUniform2fv(glGetUniformLocation(program, "tiling"), 1, glm::value_ptr(settings.tiling));
    glUniform2fv(glGetUniformLocation(program, "scrollSpeed"), 1, glm::value_ptr(settings.scrollSpeed));

    glUniform1f(glGetUniformLocation(program, "reflectivity"), settings.reflectivity);

    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    glUniform1f(glGetUniformLocation(program, "time"), currentTime);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}