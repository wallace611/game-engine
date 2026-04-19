#include "camera.h"

#include <cmath>
#include <GL/freeglut.h> 

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() {
    fov = 60.0f;
    zNear = 0.9f;
    zFar = 1000.0f;
    
    // Default position
    localPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    
    // Set default movement speed (e.g., 5.0 units per second)
    movSpeed = glm::vec3(5.0f, 5.0f, 5.0f);
    
    // Yaw is -90.0f by default so the camera points at the -Z axis
    currentYaw = -90.0f; 
    currentPitch = 0.0f;

    // Initialize pending inputs to zero
    pendingMoveDirection = glm::vec3(0.0f);
    pendingPitchDelta = 0.0f;
    pendingYawDelta = 0.0f;

    // Calculate initial quaternion and vectors based on default Euler angles
    SetLocalRotation(glm::vec3(currentPitch, currentYaw, 0.0f));
}

void Camera::UpdateCameraVectors() {
    // 1. Define the world's default forward and up vectors
    glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // 2. Rotate the default vectors by our current quaternion rotation
    camFront = glm::normalize(localRotation * worldForward);
    camUp = glm::normalize(localRotation * worldUp);
    
    // 3. Calculate the right vector via cross product
    camRight = glm::normalize(glm::cross(camFront, camUp));
}

// ==========================================
// Getters
// ==========================================
glm::vec3 Camera::GetLocalPosition() const { return localPosition; }
glm::quat Camera::GetLocalRotation() const { return localRotation; }
glm::vec3 Camera::GetLocalRotationEuler() const { return glm::degrees(glm::eulerAngles(localRotation)); }

void Camera::SetLocalRotation(const glm::quat &rot)
{
    localRotation = rot;
    
    // Sync the explicit FPS tracking variables with the new quaternion
    glm::vec3 euler = glm::degrees(glm::eulerAngles(localRotation));
    currentPitch = euler.x;
    currentYaw = euler.y;

    UpdateCameraVectors();
}

void Camera::SetLocalRotation(const glm::vec3& eulerDegrees) {
    currentPitch = eulerDegrees.x;
    currentYaw = eulerDegrees.y;
    
    localRotation = glm::quat(glm::radians(glm::vec3(currentPitch, currentYaw, 0.0f)));
    UpdateCameraVectors();
}

void Camera::Move(const glm::vec3 direction) {
    // Accumulate movement direction (e.g., W and D pressed together)
    pendingMoveDirection += direction;
}

void Camera::Rotate(float pitch, float yaw) {
    // Accumulate rotation deltas from mouse movement
    pendingPitchDelta += pitch;
    pendingYawDelta += yaw;
}

void Camera::Ready() {}

void Camera::Update(float deltatime) {
    if (pendingPitchDelta != 0.0f || pendingYawDelta != 0.0f) {
        currentPitch += pendingPitchDelta;
        currentYaw += pendingYawDelta;

        if (currentPitch > 89.0f) currentPitch = 89.0f;
        if (currentPitch < -89.0f) currentPitch = -89.0f;

        // Apply the new rotation (roll is strictly kept at 0.0f)
        SetLocalRotation(glm::vec3(currentPitch, currentYaw, 0.0f));

        // Reset rotation inputs
        pendingPitchDelta = 0.0f;
        pendingYawDelta = 0.0f;
    }

    // Check if there is any movement input this frame
    if (glm::length(pendingMoveDirection) > 0.0f) {
        // Normalize to prevent the "diagonal speedup" issue
        glm::vec3 inputDir = glm::normalize(pendingMoveDirection);

        // X = Right/Left, Y = Up/Down, Z = Forward/Backward
        glm::vec3 velocity = (camRight * inputDir.x + 
                              camUp * inputDir.y + 
                              camFront * inputDir.z) * movSpeed * deltatime;

        // Apply absolute position change
        SetLocalPosition(localPosition + velocity);

        // Reset movement input
        pendingMoveDirection = glm::vec3(0.0f);
    }

    Object::Update(deltatime);
}

void Camera::Render() {
    glm::vec3 targetPoint = localPosition + camFront;
    glm::mat4 viewMatrix = glm::lookAt(localPosition, targetPoint, camUp);
    glMultMatrixf(glm::value_ptr(viewMatrix));
}