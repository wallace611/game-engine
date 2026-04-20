#include "color_interface.h"

ColorInterface::ColorInterface(const glm::vec3 &color) {
    baseColor = color;
    currentColor = color;
}

glm::vec3 ColorInterface::GetBaseColor() const { return baseColor; }

glm::vec3 ColorInterface::GetCurrentColor() const { return currentColor; }

void ColorInterface::SetCurrentColor(const glm::vec3 &color) {
    currentColor = color;
}
