#pragma once

#include <glm/glm.hpp>

class ColorInterface {
protected:
    glm::vec3 baseColor;
    glm::vec3 currentColor;

public:
    ColorInterface(const glm::vec3& color);
    glm::vec3 GetBaseColor() const;
    glm::vec3 GetCurrentColor() const;
    void SetCurrentColor(const glm::vec3& color);
};