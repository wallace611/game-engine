#pragma once

#include "object.h"

#include <glm/glm.hpp>

class Camera {
protected:
    float fov;
    float zNear;
    float zFar;

    // Vectors defining the camera's local coordinate system
    glm::vec3 camPosition;
    glm::vec3 camFront;
    glm::vec3 camUp;

public:
    Camera();
    glm::vec3 GetCameraPosition();
    void Ready();
    void Update(float deltatime);
    void Render();
};