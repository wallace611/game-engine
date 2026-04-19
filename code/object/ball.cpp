#include "ball.h"

#include "engine.h"

Ball::Ball(float radius) : Object() {}

void Ball::Ready() {
    this->SetGlobalPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    this->SetGlobalScale(glm::vec3(1.0f)); // Start with unit scale, collider will define actual size
    collider = new SphereCollider(glm::vec3(0.0f), 1.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.2f);
    collider->SetDebugMode(true);
    GetScene()->AddChild(collider, this);
}

void Ball::Update(float deltatime) {}
