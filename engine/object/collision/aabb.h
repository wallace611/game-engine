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

    virtual bool CollideWith(const Collider* other) const override;
    virtual bool CollideWithAABB(const AABBCollider* aabb) const override;
    virtual bool CollideWithSphere(const SphereCollider* sphere) const override;

    virtual void Ready() override;
    virtual void Update(float deltatime) override;
};