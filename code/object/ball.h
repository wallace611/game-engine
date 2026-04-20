#pragma once

#include "object/object.h"
#include "object/collision/sphere.h"

class Ball : public Object {
protected:
    SphereCollider* collider;
    glm::vec3 velocity;
public:
    Ball();
    Ball(glm::vec3& initialPosition, glm::vec3& initialVelocity);

    virtual void Ready() override;
    virtual void Update(float deltatime) override;

    void CollisionCallback(Collider* from, const HitResult& hitResult);
};