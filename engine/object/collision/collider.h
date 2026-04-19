#pragma once

#include "object/object.h"

struct HitResult {
    glm::vec3 hitPoint;
    glm::vec3 hitNormal;
    float hitDistance;
    float penetrationDepth;
};

class AABBCollider;
class SphereCollider;

class Collider : public Object {
protected:
    bool isDebugMode;

    std::function<void(const Collider* self, const Collider* from, const HitResult&)> hitCallback;
    Collider(const glm::vec4& shapeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), float faceAlpha = 0.1f);
public:
    void SetHitCallback(std::function<void(const Collider* self, const Collider* from, const HitResult&)> callback) {
        this->hitCallback = callback;
    }

    bool IsDebugModeEnabled() const;

    void SetDebugMode(bool debug);

    virtual bool CollideWith(const Collider* other) const = 0;

    virtual bool CollideWithAABB(const AABBCollider* aabb) const = 0;
    virtual bool CollideWithSphere(const SphereCollider* sphere) const = 0;
    
    static bool IsCollided(const Collider* collider1, const Collider* collider2) {
        return collider1->CollideWith(collider2);
    }

    virtual void Render() override;
};