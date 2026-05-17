#include "sphere.h"

#include "rendering/drawer/collider_drawer.h"
#include "object/collision/aabb.h"

SphereCollider::SphereCollider() : SphereCollider(glm::vec3(0.0f), 1.0f) {}

SphereCollider::SphereCollider(const glm::vec3 &center, float radius) : Collider() {
    this->center = center;
    this->radius = radius;
    name = "SphereCollider";
}

SphereCollider::SphereCollider(const glm::vec3 &center, float radius,
                               const glm::vec4 &color, float faceAlpha) : Collider(color, faceAlpha) {
    this->center = center;
    this->radius = radius;
    name = "SphereCollider";
}

glm::vec3 SphereCollider::GetCenter() const { return this->center; }

float SphereCollider::GetRadius() const { return this->radius; }

void SphereCollider::SetCenter(const glm::vec3 &center) {
    this->center = center;
}

void SphereCollider::SetRadius(float radius) {
    this->radius = radius;
}

glm::vec3 SphereCollider::GetGlobalCenter() const {
    return this->GetGlobalPosition() + this->center;
}

float SphereCollider::GetGlobalRadius() const {
    glm::vec3 globalScale = this->GetGlobalScale();
    float maxScale = std::max(globalScale.x, std::max(globalScale.y, globalScale.z));
    return this->radius * maxScale;
}

bool SphereCollider::CollideWith(const Collider *other, HitResult &hitResult) const {
    return other->CollideWithSphere(this, hitResult);
}

bool SphereCollider::CollideWithAABB(const AABBCollider *aabb, HitResult &hitResult) const {
    return aabb->CollideWithSphere(this, hitResult);
}

bool SphereCollider::CollideWithSphere(const SphereCollider *sphere, HitResult &hitResult) const {
    // Check collision channels using correct bitwise precedence
    if ((this->channelMask & sphere->GetChannelMask()) == 0) return false;

    glm::vec3 thisCenter = this->GetGlobalCenter();
    float thisRadius = this->GetGlobalRadius();

    glm::vec3 otherCenter = sphere->GetGlobalCenter();
    float otherRadius = sphere->GetGlobalRadius();

    glm::vec3 diff = otherCenter - thisCenter;
    float distance = glm::length(diff); 
    float radiusSum = thisRadius + otherRadius;

    // Collision check
    if (distance <= radiusSum) {
        // Calculate penetration depth and distance
        hitResult.penetrationDepth = radiusSum - distance;
        hitResult.hitDistance = distance;

        // Calculate hit normal and hit point
        if (distance == 0.0f) {
            // Edge case: Spheres are exactly overlapping at the exact same global position
            hitResult.hitNormal = glm::vec3(0.0f, 1.0f, 0.0f); // Push upward to resolve
            hitResult.hitPoint = thisCenter;
        } else {
            // Normal points from 'other' to 'this'
            hitResult.hitNormal = diff / distance; 
            // The hit point is located exactly at the boundary of the 'other' sphere
            hitResult.hitPoint = otherCenter + hitResult.hitNormal * otherRadius;
        }
        hitResult.hitChannel = this->channelMask & sphere->GetChannelMask();
        return true;
    }
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
    Object::Update(deltatime);
}
