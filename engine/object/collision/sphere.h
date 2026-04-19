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

    virtual bool CollideWith(const Collider* other) const override;
    virtual bool CollideWithAABB(const AABBCollider* aabb) const override;
    virtual bool CollideWithSphere(const SphereCollider* sphere) const override;

    virtual void Ready() override;
    virtual void Update(float deltatime) override;
};