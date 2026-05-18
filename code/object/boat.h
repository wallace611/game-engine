#pragma once

#include "object/object.h"
#include "component/rigidbody.h"

class Boat : public Object {
protected:
    std::vector<glm::vec3> buoyancyPoints;
    std::vector<Object*> debugPointMarkers;
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f);
    float mass = 120.0f;
    Water* waterRef;
    bool isSleeping = false;
    float sleepTimer = 0.0f;
    bool showDebugPoints = true;
public:
    Boat(Water* waterRef, bool showBuoyancyPoints = true);

    void Update(float deltatime);
};