#include "camera_settings.h"

#include "engine.h"

void NextCamera() {
    Camera* cam = GetScene()->GetCamera();
    if (!cam) return;
    cameraIndex = (cameraIndex + 1) % 4;
    switch (cameraIndex) {
    case 0:
        cam->SetProjectionMode(CAMERA_PERSPECTIVE);
        cam->SetIsMovable(true);
        break;
    
    case 1:
        cam->SetProjectionMode(CAMERA_ORTHOGRAPHIC);
        cam->SetIsMovable(false);
        cam->SetGlobalPosition(glm::vec3(0.0f, 21.0f, 0.0f));
        cam->SetGlobalRotation(glm::vec3(270.0f, 0.0f, 0.0f));
        break;
    
    case 2:
        cam->SetGlobalPosition(glm::vec3(10.0f, 9.0f, 0.0f));
        cam->SetGlobalRotation(glm::vec3(0.0f, 90.0f, 0.0f));
        break;

    case 3:
        cam->SetGlobalPosition(glm::vec3(0.0f, 9.0f, 10.0f));
        cam->SetGlobalRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        break;
    }
}