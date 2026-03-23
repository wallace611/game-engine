#include "light_object.h"

LightObject::LightObject() {
}

glm::vec3 LightObject::GetLightColor()
{
    return lightColor;
}

void LightObject::SetLightColor(const glm::vec3& color) {
    lightColor = color;
}
