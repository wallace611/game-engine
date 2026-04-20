#pragma once

#include "object/object.h"
#include "object/collision/aabb.h"
#include "interface/color_interface.h"

class Wall : public Object, public ColorInterface {
public:
    Wall(int x, int y, int z, const glm::vec3& color1, const glm::vec3& color2);
    Wall(glm::vec3& size, glm::vec3& mainColor);

    virtual void Ready() override;
};