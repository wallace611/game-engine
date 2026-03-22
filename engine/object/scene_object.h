#pragma once

#include "object.h"
#include "camera.h"

class Scene : public Object {
private:
    Camera* renderCamera;

    Object* lightSource;
public:
    Scene();
    void Ready();
    void Update(float deltatime);
    void Render();

    Camera* GetCamera();
    Object* GetLightSource();
    int SetCamera(Camera* cam);
};