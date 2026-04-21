#pragma once

#include "object/object.h"
#include "interface/color_interface.h"

class Car : public Object, public ColorInterface {
public:
    Car(const glm::vec3& mainColor);
};