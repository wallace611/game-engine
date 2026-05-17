#pragma once

#include "object/water.h"

class RigidBody {
protected:
    Water* waterRef;
    Object* owner;
public:
    RigidBody(Water* waterRef, Object* owner);

    glm::vec3 GetSphereBuoyancyVelocity(const glm::vec3& center, float mass, float radius) const;
    glm::vec3 GetSphereDragAcceleration(const glm::vec3& center, float mass, const glm::vec3& velocity, float radius) const;
    glm::vec3 GetPointBuoyancyVelocity(const glm::vec3& point, float mass, float volume) const;
    glm::vec3 GetPointDragAcceleration(const glm::vec3& point, float mass, const glm::vec3& velocity, float volume) const;
};