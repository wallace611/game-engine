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
    float baseMass = 120.0f;
    float metalCubeMass = 50.0f;
    Object* metalCube = nullptr;
    Water* waterRef;
    bool isSleeping = false;
    float sleepTimer = 0.0f;
    bool showDebugPoints = true;
public:
    Boat(Water* waterRef, bool showBuoyancyPoints = true);
    void Update(float deltatime);

    void AddForce(glm::vec3 position, glm::vec3 force);
    float GetTotalMass() const;
    glm::vec3 GetCOMLocal() const;
    Object* GetMetalCube() const { return metalCube; }
    void SetMetalCubeMass(float mass) { metalCubeMass = glm::max(mass, 0.1f); }
    float GetMetalCubeMass() const { return metalCubeMass; }
};