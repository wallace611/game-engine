#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include <random>

#include "object/water.h"

class Object;

static inline float RandomNoise(float magnitude) {
    static thread_local std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return dist(rng) * magnitude;
}

class RigidBody {
public:
    RigidBody() = delete;

    template<typename WaterT, typename OwnerT>
    static glm::vec3 GetSphereBuoyancyVelocity(WaterT* waterRef, OwnerT* owner, const glm::vec3& center, float mass, float radius) {
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
        const float pi = 3.14159265358979323846f;
        if (h >= fullDepth) {
            displacedVolume = 4.0f / 3.0f * pi * radius * radius * radius;
        } else {
            displacedVolume = pi * submergedHeight * submergedHeight * (3.0f * radius - submergedHeight) / 3.0f;
        }

        const float fluidDensity = waterRef->GetDensity();
        const float gravityAccel = 9.81f;
        const float buoyantForce = fluidDensity * displacedVolume * gravityAccel;
        const float buoyantAcceleration = buoyantForce / mass;

        return glm::vec3(RandomNoise(0.3f), buoyantAcceleration + RandomNoise(0.4f), RandomNoise(0.3f));
    }

    template<typename WaterT, typename OwnerT>
    static glm::vec3 GetSphereDragAcceleration(WaterT* waterRef, OwnerT* owner, const glm::vec3& center, float mass, const glm::vec3& velocity, float radius) {
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
        const float pi = 3.14159265358979323846f;
        if (h >= fullDepth) {
            displacedVolume = 4.0f / 3.0f * pi * radius * radius * radius;
        } else {
            displacedVolume = pi * submergedHeight * submergedHeight * (3.0f * radius - submergedHeight) / 3.0f;
        }

        const float fluidDensity = waterRef->GetDensity();
        const float dragCoefficient = 1.5f;
        const float dragAcceleration = dragCoefficient * displacedVolume * fluidDensity / mass;

        return -dragAcceleration * velocity;
    }

    template<typename WaterT, typename OwnerT>
    static glm::vec3 GetPointBuoyancyVelocity(WaterT* waterRef, OwnerT* owner, const glm::vec3& point,
                                              float mass, float volume) {
        if (!waterRef) {
            return glm::vec3(0.0f);
        }

        const float waterLevel = waterRef->GetYLevel();
        
        if (point.y >= waterLevel) {
            return glm::vec3(0.0f);
        }

        const float fluidDensity = waterRef->GetDensity();
        const float gravityAccel = 9.81f;
        const float buoyantForce = fluidDensity * volume * gravityAccel;
        const float buoyantAcceleration = buoyantForce / mass;

        return glm::vec3(0.0f, buoyantAcceleration, 0.0f);
    }

    template<typename WaterT, typename OwnerT>
    static glm::vec3 GetPointDragAcceleration(WaterT* waterRef, OwnerT* owner, const glm::vec3& point,
                                              float mass,
                                              const glm::vec3& velocity,
                                              float volume) {
        if (!waterRef) {
            return glm::vec3(0.0f);
        }

        const float waterLevel = waterRef->GetYLevel();
        
        if (point.y >= waterLevel) {
            return glm::vec3(0.0f);
        }

        const float fluidDensity = waterRef->GetDensity();
        const float dragCoefficient = 1.5f;
        const float dragAcceleration = dragCoefficient * volume * fluidDensity / mass;

        return -dragAcceleration * velocity;
    }
};