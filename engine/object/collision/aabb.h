#pragma once

#include "collider.h"

class AABBCollider : public Collider {
protected:
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
public:
    AABBCollider();
    AABBCollider(const glm::vec3& minPt, const glm::vec3& maxPt);
    AABBCollider(const glm::vec3& minPt, const glm::vec3& maxPt, const glm::vec4& color, float faceAlpha);

    glm::vec3 GetMinPoint() const;
    glm::vec3 GetMaxPoint() const;
    void SetMinPoint(const glm::vec3& minPt);
    void SetMaxPoint(const glm::vec3& maxPt);
    glm::vec3 GetGlobalMinPoint() const;
    glm::vec3 GetGlobalMaxPoint() const;

    virtual bool CollideWith(const Collider* other, HitResult& hitResult) const override;
    virtual bool CollideWithAABB(const AABBCollider* aabb, HitResult& hitResult) const override;
    virtual bool CollideWithSphere(const SphereCollider* sphere, HitResult& hitResult) const override;

    virtual void Ready() override;
    virtual void Update(float deltatime) override;
};