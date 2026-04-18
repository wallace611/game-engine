#pragma once

#include "object/object.h"

class LightObject : public Object {
private:
    glm::vec3 lightColor;

public:
    LightObject();
    glm::vec3 GetLightColor() const;
    void SetLightColor(const glm::vec3& color);
};