#include "collider.h"

class AABBCollider : public Collider {
protected:
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
public:
    AABBCollider();
    AABBCollider(const glm::vec3& minPt, const glm::vec3& maxPt);

    glm::vec3 GetMinPoint() const;
    glm::vec3 GetMaxPoint() const;
    void SetMinPoint(const glm::vec3& minPt);
    void SetMaxPoint(const glm::vec3& maxPt);

    virtual void Ready() override;
    virtual void Update(float deltatime) override;
    virtual void Render() override;
};