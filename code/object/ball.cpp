#include "ball.h"

#include "engine.h"
#include "glm/gtc/random.hpp"
#include "rendering/drawer/phong_drawer.h"

Ball::Ball() : Ball(glm::ballRand(9.0f) + glm::vec3(0.0f, 15.0f, 0.0f), glm::ballRand(5.0f)) {}

Ball::Ball(glm::vec3 &initialPosition, glm::vec3 &initialVelocity) : Object() {
    this->SetGlobalPosition(initialPosition);
    this->velocity = initialVelocity;
    PhongDrawer* drawer = new PhongDrawer("model/sphere.ply");
    drawer->EnableCustomColor(true);
    drawer->SetCustomColor(glm::vec3(0.3f));
    this->SetDrawer(drawer);
    this->SetGlobalScale(glm::vec3(2.0f));
}

void Ball::Ready() {
    collider = new SphereCollider(glm::vec3(0.0f), 0.5f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.2f);
    collider->SetHitCallback([this](Collider* self, Collider* from, const HitResult& hitResult) {
        this->CollisionCallback(from, hitResult);
    });
    GetScene()->AddChild(collider, this);
}

void Ball::Update(float deltatime) {
    this->SetGlobalPosition(this->GetGlobalPosition() + this->velocity * deltatime);

    Object::Update(deltatime);
}

void Ball::CollisionCallback(Collider *from, const HitResult &hitResult) {
    if (glm::dot(this->velocity, hitResult.hitNormal) >= 0.0f) {
        return;
    }

    glm::vec3 correction = hitResult.hitNormal * (hitResult.penetrationDepth * 0.5f);
    this->SetGlobalPosition(this->GetGlobalPosition() + correction);

    const float restitution = 0.8f; // 1.0 = 完全彈性，0.0 = 完全非彈性
    this->velocity = glm::reflect(this->velocity, hitResult.hitNormal) * restitution;
}
