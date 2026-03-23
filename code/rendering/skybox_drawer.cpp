#include "skybox_drawer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"

SkyboxDrawer::SkyboxDrawer(const std::string &texture_path) : ModelDrawer("shader/skybox", texture_path) {}

void SkyboxDrawer::Draw()
{
    glUseProgram(program);

    // 1. Get the camera position so the skybox can follow the player
    glm::vec3 cameraPos = glm::vec3(GetScene()->GetCamera()->GetLocalPosition());
    
    // 2. Build a specific model matrix for the skybox
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // Move the center of the skybox to the camera
    modelMatrix = glm::translate(modelMatrix, cameraPos);
    // Scale it up massively so it encapsulates the whole scene
    modelMatrix = glm::scale(modelMatrix, glm::vec3(500.0f, 500.0f, 500.0f)); 

    // 3. Pass the model matrix to the skybox shader
    GLint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // 4. Bind environment panorama to Unit 0 
    // (We only need 1 texture here, no need for texture1 and equirectangularMap)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "equirectangularMap"), 0);

    // 5. Temporarily disable depth writing! 
    // This ensures the skybox is drawn as the "absolute background" 
    // and won't physically intersect with your other 3D objects.
    glDepthMask(GL_FALSE);

    // 6. Draw the cube geometry
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    glBindVertexArray(0);

    // 7. Re-enable depth writing so the rest of your scene renders correctly!
    glDepthMask(GL_TRUE);

    glUseProgram(0);
}