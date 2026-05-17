#include "ball.h"

#include <iostream>
#include "object/collision/sphere.h"
#include "rendering/drawer/phong_drawer.h"
#include "engine.h"

Ball::Ball(Water* waterRef) {
    name = "Ball";
    rigidbodyComp = new RigidBody(waterRef, this);
    this->SetDrawer(new PhongDrawer("model/sphere.ply", this));
    SphereCollider* collider = new SphereCollider(glm::vec3(0.0f), BASE_RADIUS);
    GetScene()->AddChild(collider, this);
    collider->SetHitCallback([this](Collider* self, Collider* from, const HitResult& hitResult) {
        this->Callback(from, hitResult);
    });
    collider->SetDebugMode(true);
}

void Ball::Ready() {
    Object::Ready();
}

void Ball::Update(float deltatime) {
    Object::Update(deltatime);

    velocity += gravity * deltatime;
    if (rigidbodyComp) {
        velocity += rigidbodyComp->GetSphereBuoyancyVelocity(GetGlobalPosition(), mass, GetGlobalRadius()) * deltatime;
        velocity += rigidbodyComp->GetSphereDragAcceleration(GetGlobalPosition(), mass, velocity, GetGlobalRadius()) * deltatime;
    }
    SetGlobalPosition(GetGlobalPosition() + velocity * deltatime);
}

void Ball::Callback(Collider *other, const HitResult &hitResult) {
    glm::vec3 correction = hitResult.hitNormal * (hitResult.penetrationDepth * 0.5f);
    this->SetGlobalPosition(this->GetGlobalPosition() + correction);

    const float restitution = 0.8f; 
    this->velocity = glm::reflect(this->velocity, hitResult.hitNormal) * restitution;

}
