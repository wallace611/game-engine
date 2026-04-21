#pragma once

#include "object/object.h"
#include "interface/color_interface.h"

class Man : public Object, public ColorInterface {
public:
    Man(const glm::vec3& mainColor);
};