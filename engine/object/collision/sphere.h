#pragma once

#include "collider.h"

class SphereCollider : public Collider {
protected:
    glm::vec3 center;
    float radius;
public:
    SphereCollider();
    SphereCollider(const glm::vec3& center, float radius);
    SphereCollider(const glm::vec3& center, float radius, const glm::vec4& color, float faceAlpha);

    glm::vec3 GetCenter() const;
    float GetRadius() const;
    void SetCenter(const glm::vec3& center);
    void SetRadius(float radius);
    glm::vec3 GetGlobalCenter() const;
    float GetGlobalRadius() const;

    virtual bool CollideWith(const Collider* other, HitResult& hitResult) const override;
    virtual bool CollideWithAABB(const AABBCollider* aabb, HitResult& hitResult) const override;
    virtual bool CollideWithSphere(const SphereCollider* sphere, HitResult& hitResult) const override;

    virtual void Ready() override;
    virtual void Update(float deltatime) override;
};