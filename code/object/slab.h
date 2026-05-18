#pragma once

#include "component/rigidbody.h"
#include "object/object.h"

class Slab : public Object {
protected:
    std::vector<glm::vec3> buoyancyPoints; // Local positions of buoyancy points
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f);
    float mass = 100.0f;
    Water* waterRef;
    bool isSleeping = false;
    float sleepTimer = 0.0f;
public:
    Slab(Water* waterRef);

    virtual void Update(float deltatime) override;
};