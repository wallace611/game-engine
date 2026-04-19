#pragma once

#include "object.h"
#include "camera.h"

class Collider;

class Scene : public Object {
private:
    Camera* renderCamera;

    std::vector<Collider*> colliders;
public:
    Scene();
    virtual void Ready() override;
    virtual void Update(float deltatime) override;
    void CollisionCheck();
    virtual void Render() override;

    virtual void AddChild(Object* child) override;

    Camera* GetCamera();
    int SetCamera(Camera* cam);
};