#include "rigidbody.h"

#include <algorithm>
#include <random>

static float RandomNoise(float magnitude) {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return dist(rng) * magnitude;
}

RigidBody::RigidBody(Water *waterRef, Object *owner)
    : waterRef(waterRef), owner(owner) {
}

glm::vec3 RigidBody::GetSphereBuoyancyVelocity(const glm::vec3 &center, float mass, float radius) const {
    if (!waterRef) {
        return glm::vec3(0.0f);
    }

    const float waterLevel = waterRef->GetYLevel();
    const float bottomY = center.y - radius;
    const float h = waterLevel - bottomY;

    // Case 1: completely above water
    if (h <= 0.0f) {
        return glm::vec3(0.0f);
    }

    const float fullDepth = 2.0f * radius;
    const float submergedHeight = std::min(h, fullDepth);

    float displacedVolume;
    if (h >= fullDepth) {
        // Case 2: completely submerged
        displacedVolume = 4.0f / 3.0f * 3.14159265358979323846f * radius * radius * radius;
    } else {
        // Case 3: partially submerged
        displacedVolume = 3.14159265358979323846f * submergedHeight * submergedHeight * (3.0f * radius - submergedHeight) / 3.0f;
    }

    const float fluidDensity = waterRef->GetDensity();
    const float gravityAccel = 9.81f;
    const float buoyantForce = fluidDensity * displacedVolume * gravityAccel;
    const float buoyantAcceleration = buoyantForce / mass;

    return glm::vec3(RandomNoise(0.3f), buoyantAcceleration + RandomNoise(0.4), RandomNoise(0.3f));
}

glm::vec3 RigidBody::GetSphereDragAcceleration(const glm::vec3 &center, float mass, const glm::vec3 &velocity, float radius) const {
    if (!waterRef) {
        return glm::vec3(0.0f);
    }

    const float waterLevel = waterRef->GetYLevel();
    const float bottomY = center.y - radius;
    const float h = waterLevel - bottomY;
    if (h <= 0.0f) {
        return glm::vec3(0.0f);
    }

    const float fullDepth = 2.0f * radius;
    const float submergedHeight = std::min(h, fullDepth);

    float displacedVolume;
    if (h >= fullDepth) {
        displacedVolume = 4.0f / 3.0f * 3.14159265358979323846f * radius * radius * radius;
    } else {
        displacedVolume = 3.14159265358979323846f * submergedHeight * submergedHeight * (3.0f * radius - submergedHeight) / 3.0f;
    }

    const float fluidDensity = waterRef->GetDensity();
    const float dragCoefficient = 1.5f;
    const float dragAcceleration = dragCoefficient * displacedVolume * fluidDensity / mass;

    return -dragAcceleration * velocity;
}

glm::vec3 RigidBody::GetPointBuoyancyVelocity(const glm::vec3 &point,
                                              float mass, float volume) const {
    if (!waterRef) {
        return glm::vec3(0.0f);
    }

    const float waterLevel = waterRef->GetYLevel();
    
    // Check if point is below water surface
    if (point.y >= waterLevel) {
        return glm::vec3(0.0f);
    }

    const float fluidDensity = waterRef->GetDensity();
    const float gravityAccel = 9.81f;
    const float buoyantForce = fluidDensity * volume * gravityAccel;
    const float buoyantAcceleration = buoyantForce / mass;

    return glm::vec3(0.0f, buoyantAcceleration + RandomNoise(0.2f), 0.0f);
}

glm::vec3 RigidBody::GetPointDragAcceleration(const glm::vec3 &point,
                                              float mass,
                                              const glm::vec3 &velocity,
                                              float volume) const {
    if (!waterRef) {
        return glm::vec3(0.0f);
    }

    const float waterLevel = waterRef->GetYLevel();
    
    // Check if point is below water surface
    if (point.y >= waterLevel) {
        return glm::vec3(0.0f);
    }

    const float fluidDensity = waterRef->GetDensity();
    const float dragCoefficient = 1.5f;
    const float dragAcceleration = dragCoefficient * volume * fluidDensity / mass;

    return -dragAcceleration * velocity;
}
