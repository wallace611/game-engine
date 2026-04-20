#pragma once

#include "rendering/model_drawer.h"
#include "object/object.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class PhongDrawer : public ModelDrawer {
private:
    // Light properties (Position must be in View Space for compatibility mode)
    glm::vec3 lightColor;
    Object* parent;

    // Attenuation parameters
    float constant;
    float linear;
    float quadratic;

    // Color override interface
    bool useCustomColor;
    glm::vec3 customColor;

public:
    PhongDrawer(const std::string& model_path, Object* parent);
    PhongDrawer(const std::string& model_path, const std::string& texture_path, Object* parent);

    void Draw() override;

    // Set attenuation parameters
    void SetAttenuation(float c, float l, float q);

    // Override entire model color (ignores texture if enabled)
    void EnableCustomColor(bool enable);
    void SetCustomColor(const glm::vec3& color);
};