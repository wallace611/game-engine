#pragma once

#include "object.h"
#include "camera.h"

class Collider;

class Scene : public Object {
protected:
    Camera* renderCamera;

    std::vector<Collider*> dynamicCollider;
    std::vector<Collider*> staticCollider;
    Object* light;

public:
    Scene();
    virtual void Ready() override;
    virtual void Update(float deltatime) override;
    virtual void CollisionCheck();
    virtual void Render() override;

    void AddChild(Object* child, Object* parent = nullptr, bool isDynamic = true);

    Object* GetLight() { return light; }
    Camera* GetCamera();
    int SetCamera(Camera* cam);
};