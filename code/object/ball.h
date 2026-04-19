#pragma once

#include "object/object.h"
#include "object/collision/sphere.h"

class Ball : public Object {
protected:
    SphereCollider* collider;
public:
    Ball(float radius);

    virtual void Ready() override;
    virtual void Update(float deltatime) override;
};