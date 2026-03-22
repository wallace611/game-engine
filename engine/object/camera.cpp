#include "camera.h"

#include <cmath>
#include <GL/freeglut.h> 

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() {
    fov = 60.0f;
    zNear = 0.9f;
    zFar = 1000.0f;
    
    camPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    
    camFront = glm::vec3(0.0f, 0.0f, -1.0f);
    
    camUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Camera::GetCameraPosition() {
    return glm::vec3(camPosition);
}

void Camera::Ready() {
    // Initialization logic if needed in the future
}

void Camera::Update(float deltatime) {
    // Input handling and movement logic will go here
}

void Camera::Render() {
    // Calculate the exact point in 3D space the camera is looking at
    glm::vec3 targetPoint = camPosition + camFront;

    // Generate the View Matrix using glm::lookAt
    // It creates a right-handed view matrix automatically
    glm::mat4 viewMatrix = glm::lookAt(camPosition, targetPoint, camUp);

    // Apply the View Matrix directly to the OpenGL state machine.
    glMultMatrixf(glm::value_ptr(viewMatrix));
}