#include "aabb.h"

#include "rendering/drawer/collider_drawer.h"
#include "object/collision/sphere.h"

#include <glm/gtc/type_ptr.hpp>

AABBCollider::AABBCollider() : AABBCollider(glm::vec3(0.0f), glm::vec3(0.0f)) {}

AABBCollider::AABBCollider(const glm::vec3 &minPt, const glm::vec3 &maxPt) : Collider() {
    this->minPoint = minPt;
    this->maxPoint = maxPt;
    name = "AABBCollider";
}

AABBCollider::AABBCollider(const glm::vec3 &minPt, const glm::vec3 &maxPt,
                           const glm::vec4 &color, float faceAlpha) : Collider(color, faceAlpha) {
    this->minPoint = minPt;
    this->maxPoint = maxPt;
    name = "AABBCollider";
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

glm::vec3 AABBCollider::GetGlobalMinPoint() const {
    glm::vec3 globalPos = this->GetGlobalPosition();
    glm::vec3 globalScale = this->GetGlobalScale();
    return globalPos + this->minPoint * globalScale;
}

glm::vec3 AABBCollider::GetGlobalMaxPoint() const {
    glm::vec3 globalPos = this->GetGlobalPosition();
    glm::vec3 globalScale = this->GetGlobalScale();
    return globalPos + this->maxPoint * globalScale;
}

bool AABBCollider::CollideWith(const Collider *other, HitResult &hitResult) const {
    return other->CollideWithAABB(this, hitResult);
}

bool AABBCollider::CollideWithAABB(const AABBCollider *aabb, HitResult &hitResult) const {
    if ((this->channelMask & aabb->GetChannelMask()) == 0) return false;

    glm::vec3 thisMin = this->GetGlobalMinPoint();
    glm::vec3 thisMax = this->GetGlobalMaxPoint();
    glm::vec3 otherMin = aabb->GetGlobalMinPoint();
    glm::vec3 otherMax = aabb->GetGlobalMaxPoint();

    float dx1 = thisMax.x - otherMin.x; 
    float dx2 = otherMax.x - thisMin.x; 
    float dy1 = thisMax.y - otherMin.y;
    float dy2 = otherMax.y - thisMin.y;
    float dz1 = thisMax.z - otherMin.z;
    float dz2 = otherMax.z - thisMin.z;

    // No overlap if any of these is negative
    if (dx1 < 0 || dx2 < 0 || dy1 < 0 || dy2 < 0 || dz1 < 0 || dz2 < 0) {
        return false;
    }

    // Find the minimum overlap along each axis to determine penetration depth and normal
    float minOverlap = dx1;
    glm::vec3 normal(-1.0f, 0.0f, 0.0f); 

    if (dx2 < minOverlap) { minOverlap = dx2; normal = glm::vec3(1.0f, 0.0f, 0.0f); }
    if (dy1 < minOverlap) { minOverlap = dy1; normal = glm::vec3(0.0f, -1.0f, 0.0f); }
    if (dy2 < minOverlap) { minOverlap = dy2; normal = glm::vec3(0.0f, 1.0f, 0.0f); }
    if (dz1 < minOverlap) { minOverlap = dz1; normal = glm::vec3(0.0f, 0.0f, -1.0f); }
    if (dz2 < minOverlap) { minOverlap = dz2; normal = glm::vec3(0.0f, 0.0f, 1.0f); }

    hitResult.penetrationDepth = minOverlap;
    hitResult.hitNormal = normal;
    hitResult.hitDistance = 0.0f;
    hitResult.hitChannel = this->channelMask & aabb->GetChannelMask();

    glm::vec3 overlapMin = glm::max(thisMin, otherMin);
    glm::vec3 overlapMax = glm::min(thisMax, otherMax);
    hitResult.hitPoint = (overlapMin + overlapMax) * 0.5f;

    return true;
}

bool AABBCollider::CollideWithSphere(const SphereCollider *sphere, HitResult &hitResult) const {
    // Check collision channels using correct bitwise precedence
    if ((this->channelMask & sphere->GetChannelMask()) == 0) return false;

    glm::vec3 globalCenter = sphere->GetGlobalCenter();
    float globalRadius = sphere->GetGlobalRadius();

    glm::vec3 globalMin = this->GetGlobalMinPoint();
    glm::vec3 globalMax = this->GetGlobalMaxPoint();

    // Find the closest point on the AABB to the sphere's center in global space
    glm::vec3 closestPoint = glm::clamp(globalCenter, globalMin, globalMax);
    
    // Vector pointing from Sphere Center to the Closest Point on AABB
    glm::vec3 diff = closestPoint - globalCenter;
    float distance = glm::length(diff);

    // Collision check
    if (distance <= globalRadius) {
        hitResult.hitPoint = closestPoint;
        hitResult.hitDistance = distance;

        if (distance > 0.0001f) {
            // Case A: The sphere center is outside the AABB.
            hitResult.hitNormal = diff / distance; 
            hitResult.penetrationDepth = globalRadius - distance;
        } else {
            // Case B: Deep Penetration. The sphere center is inside the AABB.
            float dx1 = globalCenter.x - globalMin.x;
            float dx2 = globalMax.x - globalCenter.x;
            float dy1 = globalCenter.y - globalMin.y;
            float dy2 = globalMax.y - globalCenter.y;
            float dz1 = globalCenter.z - globalMin.z;
            float dz2 = globalMax.z - globalCenter.z;

            float minOverlap = dx1;
            hitResult.hitNormal = glm::vec3(1.0f, 0.0f, 0.0f); // Default push to +X
            
            if (dx2 < minOverlap) { minOverlap = dx2; hitResult.hitNormal = glm::vec3(-1.0f, 0.0f, 0.0f); }
            if (dy1 < minOverlap) { minOverlap = dy1; hitResult.hitNormal = glm::vec3(0.0f, 1.0f, 0.0f); }
            if (dy2 < minOverlap) { minOverlap = dy2; hitResult.hitNormal = glm::vec3(0.0f, -1.0f, 0.0f); }
            if (dz1 < minOverlap) { minOverlap = dz1; hitResult.hitNormal = glm::vec3(0.0f, 0.0f, 1.0f); }
            if (dz2 < minOverlap) { minOverlap = dz2; hitResult.hitNormal = glm::vec3(0.0f, 0.0f, -1.0f); }

            hitResult.penetrationDepth = globalRadius + minOverlap;
        }
        hitResult.hitChannel = this->channelMask & sphere->GetChannelMask();
        return true;
    }
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
    Object::Update(deltatime);
}

