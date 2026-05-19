#include "boat.h"

#include "engine.h"
#include "rendering/drawer/phong_drawer.h"

Boat::Boat(Water *waterRef, bool showBuoyancyPoints) {
    name = "Boat";
    this->waterRef = waterRef;
    this->showDebugPoints = showBuoyancyPoints;

    Object* render = new Object();
    GetScene()->AddChild(render, this);
    render->SetDrawer(new PhongDrawer("model/boat.ply", "textures/boat.png", render));
    render->SetLocalScale(glm::vec3(2.0f));
    render->SetLocalRotation(glm::vec3(-90.0f, 0.0f, 0.0f));

    // Create a grid of buoyancy sample points in local object space.
    for (int x = -2; x <= 2; ++x) {
        for (int z = -1; z <= 1; ++z) {
            buoyancyPoints.emplace_back(glm::vec3(0.35f * x, 0.0f, 0.35f * z));
        }
    }

    if (showDebugPoints) {
        for (const auto& localPos : buoyancyPoints) {
            Object* marker = new Object();
            marker->name = "BuoyancyMarker";
            marker->SetLocalPosition(localPos);
            marker->SetLocalScale(glm::vec3(0.08f));
            marker->SetDrawer(new PhongDrawer("model/sphere.ply", marker));
            GetScene()->AddChild(marker, this);
            debugPointMarkers.push_back(marker);
        }
    }

    Object* metalCube_obj = new Object();
    GetScene()->AddChild(metalCube_obj, this);
    metalCube_obj->SetDrawer(new PhongDrawer("model/cube.ply", "textures/iron_block.png", metalCube_obj));
    metalCube_obj->SetLocalScale(glm::vec3(0.4f));
    metalCube_obj->SetLocalPosition(glm::vec3(0.0f, 0.3f, 0.0f));
    metalCube = metalCube_obj;
}

void Boat::AddForce(glm::vec3 position, glm::vec3 force) {
    glm::vec3 boatCenter = GetGlobalPosition();
    glm::vec3 globalScale = GetGlobalScale();

    glm::vec3 leverArm = position - boatCenter;

    // Apply linear impulse (force treated as instantaneous impulse)
    float totalMass = GetTotalMass();
    velocity += force / totalMass;

    // Apply angular impulse using a simple scalar moment of inertia approximation
    float momentOfInertia = (1.0f / 12.0f) * totalMass * (globalScale.x * globalScale.x + globalScale.z * globalScale.z);
    glm::vec3 torque = glm::cross(leverArm, force);
    if (momentOfInertia > 0.0f) {
        angularVelocity += torque / momentOfInertia;
    }

    // Wake the boat if it was sleeping
    isSleeping = false;
    sleepTimer = 0.0f;
}

float Boat::GetTotalMass() const {
    return baseMass + metalCubeMass;
}

glm::vec3 Boat::GetCOMLocal() const {
    float totalMass = GetTotalMass();
    if (totalMass <= 0.0f) return glm::vec3(0.0f);
    
    // Base mass assumed at local origin (0,0,0)
    // Metal cube at its local position
    glm::vec3 metalCubeLocalPos = metalCube ? metalCube->GetLocalPosition() : glm::vec3(0.0f);
    glm::vec3 com = metalCubeLocalPos * metalCubeMass / totalMass;
    return com;
}

void Boat::Update(float deltatime) {
    Object::Update(deltatime);

    if (!waterRef) {
        return;
    }

    glm::vec3 boatCenter = GetGlobalPosition();
    glm::quat currentRot = GetGlobalRotationQuat();
    glm::vec3 globalScale = GetGlobalScale();

    glm::vec3 comLocal = GetCOMLocal();
    glm::vec3 comGlobal = boatCenter + currentRot * (comLocal * globalScale);

    glm::vec3 totalAcceleration = gravity;
    glm::vec3 totalTorque = glm::vec3(0.0f);

    const float boatVolume = globalScale.x * globalScale.y * globalScale.z;
    const float pointVolume = boatVolume / static_cast<float>(buoyancyPoints.size());
    const float waterLevel = waterRef->GetYLevel();
    float totalMass = GetTotalMass();

    for (const auto& localPos : buoyancyPoints) {
        glm::vec3 localScaled = localPos * globalScale;
        glm::vec3 globalPos = boatCenter + currentRot * localScaled;

        glm::vec3 leverArm = globalPos - comGlobal; // from COM to sample point

        glm::vec3 pointVelocity = velocity + glm::cross(angularVelocity, leverArm);
        glm::vec3 buoyancyAccel = RigidBody::GetPointBuoyancyVelocity(waterRef, this, globalPos, totalMass, pointVolume);
        glm::vec3 dragAccel = RigidBody::GetPointDragAcceleration(waterRef, this, globalPos, totalMass, pointVelocity, pointVolume);

        float submergedFraction = 0.0f;
        float depth = waterLevel - globalPos.y;
        if (depth > 0.0f) {
            float charHeight = std::max(globalScale.y * 0.5f, 0.001f);
            submergedFraction = glm::clamp(depth / charHeight, 0.0f, 1.0f);
        }

        buoyancyAccel *= submergedFraction;
        dragAccel *= submergedFraction;

        glm::vec3 pointTotalAccel = buoyancyAccel + dragAccel;
        totalAcceleration += pointTotalAccel;

        glm::vec3 forcePerPoint = pointTotalAccel * totalMass;
        totalTorque += glm::cross(leverArm, forcePerPoint);
    }

    velocity += totalAcceleration * deltatime;
    float momentOfInertia = (1.0f / 12.0f) * totalMass * (globalScale.x * globalScale.x + globalScale.z * globalScale.z);
    angularVelocity += (totalTorque / momentOfInertia) * deltatime;

    const float linearDamping = 0.5f;
    const float angularDamping = 0.8f;
    velocity *= glm::clamp(1.0f - linearDamping * deltatime, 0.0f, 1.0f);
    angularVelocity *= glm::clamp(1.0f - angularDamping * deltatime, 0.0f, 1.0f);

    const float sleepVelocity = 0.05f;
    const float sleepAngularVel = 0.02f;
    const float sleepAcceleration = 0.1f;
    const float sleepTimeThreshold = 0.25f;

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
        float angularSpeed = glm::length(angularVelocity);
        if (angularSpeed > 0.0001f) {
            glm::vec3 rotationAxis = glm::normalize(angularVelocity);
            float rotationAngle = angularSpeed * deltatime;
            glm::quat deltaRot = glm::angleAxis(rotationAngle, rotationAxis);
            SetGlobalRotation(deltaRot * currentRot);
        }
    }
}
