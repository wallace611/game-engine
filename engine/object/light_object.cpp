#include "light_object.h"

LightObject::LightObject() {
}

glm::vec3 LightObject::GetLightColor() const
{
    return lightColor;
}

void LightObject::SetLightColor(const glm::vec3& color) {
    lightColor = color;
}
