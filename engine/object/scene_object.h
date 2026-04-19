#pragma once

#include "object.h"
#include "camera.h"

class Scene : public Object {
private:
    Camera* renderCamera;

public:
    Scene();
    virtual void Ready() override;
    virtual void Update(float deltatime) override;
    virtual void Render() override;

    Camera* GetCamera();
    int SetCamera(Camera* cam);
};