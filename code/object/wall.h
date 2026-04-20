#pragma once

#include "object/object.h"
#include "object/collision/aabb.h"

class Wall : public Object {
protected:
    AABBCollider* collider;
public:
    Wall(const glm::vec3& position, const glm::vec3& scale);

    virtual void Ready() override;
};