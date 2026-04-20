#pragma once

#include "object.h"
#include "camera.h"

class Collider;

class Scene : public Object {
private:
    Camera* renderCamera;

    std::vector<Collider*> colliders;
    Object* light;
public:
    Scene();
    virtual void Ready() override;
    virtual void Update(float deltatime) override;
    void CollisionCheck();
    virtual void Render() override;

    void AddChild(Object* child, Object* parent = nullptr);

    Object* GetLight() { return light; }
    Camera* GetCamera();
    int SetCamera(Camera* cam);
};