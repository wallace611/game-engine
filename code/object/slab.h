#pragma once

#include "component/rigidbody.h"
#include "object/object.h"

class Slab : public Object {
protected:
    std::vector<std::pair<RigidBody*, glm::vec3>> buoyancyPoints; // Local positions of buoyancy points and their offsets
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f);
    float mass = 200.0f;
public:
    Slab(Water* waterRef);

    virtual void Update(float deltatime) override;
};