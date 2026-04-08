#include "skybox_drawer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"

SkyboxDrawer::SkyboxDrawer(const std::string &texture_path) : ModelDrawer("shader/skybox", texture_path) {}

void SkyboxDrawer::Draw()
{
    glUseProgram(program);

    glm::vec3 cameraPos = glm::vec3(GetScene()->GetCamera()->GetLocalPosition());
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // Move the center of the skybox to the camera
    modelMatrix = glm::translate(modelMatrix, cameraPos);
    // Scale it up massively so it encapsulates the whole scene
    modelMatrix = glm::scale(modelMatrix, glm::vec3(500.0f, 500.0f, 500.0f)); 

    GLint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "equirectangularMap"), 0);

    // This ensures the skybox is drawn as the "absolute background" 
    // and won't physically intersect with your other 3D objects.
    glDepthMask(GL_FALSE);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);

    glUseProgram(0);
}