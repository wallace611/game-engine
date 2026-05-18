#include "slab.h"

#include <glm/glm.hpp>
#include "engine.h"
#include "rendering/drawer/phong_drawer.h"

Slab::Slab(Water *waterRef) {
    name = "Slab";
    Object* slabRender = new Object();
    slabRender->name = "SlabMesh";
    GetScene()->AddChild(slabRender, this);
    slabRender->SetLocalScale(glm::vec3(1.0f, 0.3f, 1.0f));
    slabRender->SetDrawer(new PhongDrawer("model/cube.ply", slabRender));

    this->waterRef = waterRef;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            buoyancyPoints.emplace_back(glm::vec3((i - 1) * 0.5f, 0.0f, (j - 1) * 0.5f));
        }
    }
}

void Slab::Update(float deltatime) {
    Object::Update(deltatime);

    glm::vec3 slabCenter = GetGlobalPosition();
    glm::quat currentRot = GetGlobalRotationQuat(); // Get current global rotation
    glm::vec3 globalScale = GetGlobalScale();
    
    // Assume 'gravity' is a member variable (e.g., glm::vec3(0.0f, -9.81f, 0.0f))
    glm::vec3 totalAcceleration = gravity; 
    glm::vec3 totalTorque = glm::vec3(0.0f);
    
    const float slabVolume = globalScale.x * globalScale.y * globalScale.z;
    const float pointVolume = slabVolume / static_cast<float>(buoyancyPoints.size());

    const float waterLevel = waterRef ? waterRef->GetYLevel() : -INFINITY;
    for (const auto& localPos : buoyancyPoints) {
        // [FIX 1] Transform local position to global properly (Apply Rotation & Scale)
        glm::vec3 leverArm = currentRot * (localPos * globalScale);
        glm::vec3 globalPos = slabCenter + leverArm;
        
        // [FIX 2] Calculate true point velocity (Linear Velocity + Angular Velocity effect)
        // v_point = v_linear + (omega x r)
        glm::vec3 pointVelocity = velocity + glm::cross(angularVelocity, leverArm);
        
        // Calculate buoyancy and drag accelerations using the TRUE point velocity
        glm::vec3 buoyancyAccel = RigidBody::GetPointBuoyancyVelocity(waterRef, this, globalPos, mass, pointVolume);
        glm::vec3 dragAccel = RigidBody::GetPointDragAcceleration(waterRef, this, globalPos, mass, pointVelocity, pointVolume);

        // Smooth the buoyancy/drag application by scaling with a submerged fraction
        // This prevents abrupt on/off forces when a sampled point crosses the surface.
        float submergedFraction = 0.0f;
        if (waterRef) {
            float depth = waterLevel - globalPos.y; // positive when submerged
            // Use slab thickness as a characteristic height to normalize fraction
            float charHeight = std::max(globalScale.y, 0.001f);
            submergedFraction = glm::clamp(depth / charHeight, 0.0f, 1.0f);
        }
        buoyancyAccel *= submergedFraction;
        dragAccel *= submergedFraction;
        
        // Accumulate linear acceleration
        glm::vec3 pointTotalAccel = buoyancyAccel + dragAccel;
        totalAcceleration += pointTotalAccel;
        
        // [FIX 3] Calculate torque: τ = r × F
        // In your RigidBody class, GetPointBuoyancyVelocity returns acceleration: (Force / TotalMass).
        // Therefore, the true Force at this point is simply (Acceleration * TotalMass).
        glm::vec3 forcePerPoint = pointTotalAccel * mass; 
        totalTorque += glm::cross(leverArm, forcePerPoint);
    }

    // Update linear velocity and position
    velocity += totalAcceleration * deltatime;
    float momentOfInertia = (1.0f / 12.0f) * mass * (globalScale.x * globalScale.x + globalScale.z * globalScale.z);
    angularVelocity += (totalTorque / momentOfInertia) * deltatime;

    const float linearDamping = 0.5f;
    const float angularDamping = 0.8f;
    
    velocity *= glm::clamp(1.0f - linearDamping * deltatime, 0.0f, 1.0f);
    angularVelocity *= glm::clamp(1.0f - angularDamping * deltatime, 0.0f, 1.0f);

    const float sleepVelocity = 0.05f;
    const float sleepAngularVel = 0.02f;
    const float sleepAcceleration = 0.1f;
    const float sleepTimeThreshold = 0.25f; // seconds

    // Sleep/wake logic: require object to be near-rest for a short time before sleeping
    if (glm::length(velocity) < sleepVelocity && glm::length(angularVelocity) < sleepAngularVel && glm::length(totalAcceleration) < sleepAcceleration) {
        sleepTimer += deltatime;
        if (sleepTimer >= sleepTimeThreshold) {
            isSleeping = true;
            velocity = glm::vec3(0.0f);
            angularVelocity = glm::vec3(0.0f);
        }
    } else {
        sleepTimer = 0.0f;
        isSleeping = false;
    }

    if (!isSleeping) {
        SetGlobalPosition(GetGlobalPosition() + velocity * deltatime);
    }
    
    float angularSpeed = glm::length(angularVelocity);
    if (!isSleeping && angularSpeed > 0.0001f) {
        glm::vec3 rotationAxis = glm::normalize(angularVelocity);
        float rotationAngle = angularSpeed * deltatime;
        glm::quat deltaRot = glm::angleAxis(rotationAngle, rotationAxis);
        SetGlobalRotation(deltaRot * currentRot);
    }
}
