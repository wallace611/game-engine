#include "slab.h"

#include <glm/glm.hpp>
#include "engine.h"
#include "rendering/drawer/phong_drawer.h"

Slab::Slab(Water *waterRef) {
    Object* slabRender = new Object();
    GetScene()->AddChild(slabRender, this);
    slabRender->SetLocalScale(glm::vec3(1.0f, 0.3f, 1.0f));
    slabRender->SetDrawer(new PhongDrawer("model/cube.ply", slabRender));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            RigidBody* rb = new RigidBody(waterRef, this);
            buoyancyPoints.emplace_back(rb, glm::vec3((i - 1) * 0.5f, 0.0f, (j - 1) * 0.5f));
        }
    }
}

void Slab::Update(float deltatime) {
    Object::Update(deltatime);

    glm::vec3 slabCenter = GetGlobalPosition();
    glm::vec3 totalAcceleration = gravity;
    glm::vec3 totalTorque = glm::vec3(0.0f);
    glm::vec3 globalScale = GetGlobalScale();
    const float slabVolume = globalScale.x * globalScale.y * globalScale.z;
    const float pointVolume = slabVolume / static_cast<float>(buoyancyPoints.size());

    for (auto& pair : buoyancyPoints) {
        RigidBody* rb = pair.first;
        glm::vec3 localPos = pair.second;
        
        // Convert local position to global position
        glm::vec3 globalPos = slabCenter + localPos;
        
        // Calculate buoyancy and drag accelerations
        glm::vec3 buoyancyAccel = rb->GetPointBuoyancyVelocity(globalPos, mass, pointVolume);
        glm::vec3 dragAccel = rb->GetPointDragAcceleration(globalPos, mass, velocity, pointVolume);
        
        // Accumulate linear acceleration
        totalAcceleration += (buoyancyAccel + dragAccel);
        
        // Calculate torque: τ = r × F, where F = mass * a
        glm::vec3 forcePerPoint = (buoyancyAccel + dragAccel) * (mass / buoyancyPoints.size());
        glm::vec3 leverArm = globalPos - slabCenter;
        totalTorque += glm::cross(leverArm, forcePerPoint);
    }

    // Update linear velocity
    velocity += totalAcceleration * deltatime;
    
    // Update angular velocity (simple approximation)
    // Assuming uniform inertia tensor and converting torque to angular velocity
    float momentOfInertia = 0.1f * mass; // Simple approximation for a slab
    angularVelocity += (totalTorque / momentOfInertia) * deltatime;
    
    // Apply linear motion
    SetGlobalPosition(GetGlobalPosition() + velocity * deltatime);
    
    // Apply angular motion
    float angularSpeed = glm::length(angularVelocity);
    if (angularSpeed > 0.001f) {
        glm::vec3 rotationAxis = glm::normalize(angularVelocity);
        float rotationAngle = angularSpeed * deltatime;
        glm::quat deltaRot = glm::angleAxis(rotationAngle, rotationAxis);
        SetGlobalRotation(GetGlobalRotationQuat() * deltaRot);
    }
}
