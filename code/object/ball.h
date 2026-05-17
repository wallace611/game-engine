#pragma once

#include "object/object.h"
#include "object/water.h"
#include "component/rigidbody.h"
#include "object/collision/collider.h"

class Ball : public Object {
protected:
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    const float BASE_RADIUS = 0.5f;
    float mass = 100.0f;
    RigidBody* rigidbodyComp;
public:
    Ball(Water* waterRef);

    float GetGlobalRadius() const { return BASE_RADIUS * GetGlobalScale().x; }

    virtual void Ready() override;
    virtual void Update(float deltatime) override;

    void Callback(Collider* other, const HitResult& hitResult);
};