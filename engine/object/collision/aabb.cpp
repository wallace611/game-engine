#include "aabb.h"

#include "rendering/drawer/collider_drawer.h"

AABBCollider::AABBCollider() : AABBCollider(glm::vec3(0.0f), glm::vec3(0.0f)) {}

AABBCollider::AABBCollider(const glm::vec3 &minPt, const glm::vec3 &maxPt) : Collider() {
    this->minPoint = minPt;
    this->maxPoint = maxPt;
}

AABBCollider::AABBCollider(const glm::vec3 &minPt, const glm::vec3 &maxPt,
                           const glm::vec4 &color, float faceAlpha) : Collider(color, faceAlpha) {
    this->minPoint = minPt;
    this->maxPoint = maxPt;
}

glm::vec3 AABBCollider::GetMinPoint() const {
    return this->minPoint;
}

glm::vec3 AABBCollider::GetMaxPoint() const {
    return this->maxPoint;
}

void AABBCollider::SetMinPoint(const glm::vec3 &minPt) {
    this->minPoint = minPt;
}

void AABBCollider::SetMaxPoint(const glm::vec3 &maxPt) {
    this->maxPoint = maxPt;
}

bool AABBCollider::CollideWith(const Collider *other) const {
    return other->CollideWithAABB(this);
}

bool AABBCollider::CollideWithAABB(const AABBCollider *aabb) const {
    return false;
}

bool AABBCollider::CollideWithSphere(const SphereCollider *sphere) const {
    return false;
}

void AABBCollider::Ready() {}

void AABBCollider::Update(float deltatime) {
    if (renderer) {
        ColliderDrawer* drawer = dynamic_cast<ColliderDrawer*>(renderer);
        if (drawer) {
            drawer->UpdateAABB(this->minPoint, this->maxPoint);
        }
    }
}

