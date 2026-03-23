#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTA
#include <glm/gtc/quaternion.hpp>

class Camera {
protected:
    float fov;
    float zNear;
    float zFar;

    // The absolute position and rotation of the camera
    glm::vec3 localPosition;
    glm::quat localRotation;

    // Vectors defining the camera's local coordinate system for lookAt
    glm::vec3 camFront;
    glm::vec3 camUp;
    glm::vec3 camRight;

    // Movement speed multiplier
    glm::vec3 movSpeed;

    // --- Deferred Update Variables ---
    // Stores inputs accumulated during the current frame
    glm::vec3 pendingMoveDirection;
    float pendingPitchDelta;
    float pendingYawDelta;

    // Explicitly track Euler angles to strictly prevent camera roll (Gimbal Lock safe)
    float currentYaw;
    float currentPitch;

    // Helper function to recalculate directional vectors when rotation changes
    void UpdateCameraVectors();

public:
    Camera();
    
    // --- Getters ---
    glm::vec3 GetLocalPosition() const;
    glm::quat GetLocalRotation() const;
    glm::vec3 GetLocalRotationEuler() const;

    // --- Setters (Absolute Position and Rotation) ---
    void SetLocalPosition(const glm::vec3& pos);
    void SetLocalRotation(const glm::quat& rot);
    void SetLocalRotation(const glm::vec3& eulerDegrees);

    // --- Relative Movement & Rotation (Deferred) ---
    // Call these functions anytime (e.g., inside input callbacks)
    void Move(const glm::vec3 direction);
    void Rotate(float pitch, float yaw);

    // --- Engine Loop ---
    void Ready();
    // Applies all pending inputs using deltatime, then clears them
    void Update(float deltatime); 
    void Render();
};