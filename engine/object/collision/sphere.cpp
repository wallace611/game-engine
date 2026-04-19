#include "sphere.h"

#include "rendering/drawer/collider_drawer.h"

SphereCollider::SphereCollider() : SphereCollider(glm::vec3(0.0f), 1.0f) {}

SphereCollider::SphereCollider(const glm::vec3 &center, float radius) : Collider() {
    this->center = center;
    this->radius = radius;
}

SphereCollider::SphereCollider(const glm::vec3 &center, float radius,
                               const glm::vec4 &color, float faceAlpha) : Collider(color, faceAlpha) {
    this->center = center;
    this->radius = radius;
}

glm::vec3 SphereCollider::GetCenter() const { return this->center; }

float SphereCollider::GetRadius() const { return this->radius; }

void SphereCollider::SetCenter(const glm::vec3 &center) {
    this->center = center;
}

void SphereCollider::SetRadius(float radius) {
    this->radius = radius;
}

bool SphereCollider::CollideWith(const Collider *other) const {
    return other->CollideWithSphere(this);
}

bool SphereCollider::CollideWithAABB(const AABBCollider *aabb) const {
    return false;
}

bool SphereCollider::CollideWithSphere(const SphereCollider *sphere) const {
    return false;
}

void SphereCollider::Ready() {}

void SphereCollider::Update(float deltatime) {
    if (renderer) {
        ColliderDrawer* drawer = dynamic_cast<ColliderDrawer*>(renderer);
        if (drawer) {
            drawer->UpdateSphere(this->center, this->radius);
        }
    }
}
