#pragma once

#include "object/object.h"

struct HitResult {
    glm::vec3 hitPoint;
    glm::vec3 hitNormal;
    float hitDistance;
    float penetrationDepth;
    unsigned char hitChannel;
};

#define COLLISION_CHANNEL_ALL 0xFF

class AABBCollider;
class SphereCollider;

class Collider : public Object {
protected:
    bool isDebugMode;
    unsigned char channelMask;

    std::function<void(Collider* self, Collider* from, const HitResult&)> hitCallback;
    Collider(const glm::vec4& shapeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), float faceAlpha = 0.1f);
public:
    std::function<void(Collider* self, Collider* from, const HitResult&)> GetHitCallback() const {
        return hitCallback;
    }
    void SetHitCallback(std::function<void(Collider* self, Collider* from, const HitResult&)> callback) {
        this->hitCallback = callback;
    }

    bool IsDebugModeEnabled() const;
    void SetDebugMode(bool debug);
    unsigned char GetChannelMask() const;
    void SetChannelMask(unsigned char mask);

    virtual bool CollideWith(const Collider* other, HitResult& hitResult) const = 0;

    virtual bool CollideWithAABB(const AABBCollider* aabb, HitResult& hitResult) const = 0;
    virtual bool CollideWithSphere(const SphereCollider* sphere, HitResult& hitResult) const = 0;

    static bool IsCollided(const Collider* collider1, const Collider* collider2, HitResult& hitResult) {
        return collider1->CollideWith(collider2, hitResult);
    }

    virtual void Render() override;
};