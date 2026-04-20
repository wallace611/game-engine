#pragma once

#include "rendering/model_drawer.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class PhongDrawer : public ModelDrawer {
private:
    // Light properties (Position must be in View Space for compatibility mode)
    glm::vec3 lightPosWorld;
    glm::vec3 lightColor;
    glm::vec3 viewPosWorld;

    glm::mat4 modelMatrix;

    // Attenuation parameters
    float constant;
    float linear;
    float quadratic;

    // Color override interface
    bool useCustomColor;
    glm::vec3 customColor;

public:
    PhongDrawer(const std::string& model_path);
    PhongDrawer(const std::string& model_path, const std::string& texture_path);

    void Draw() override;

    // Helper to set light using World Space coordinates and the current View Matrix
    void SetLightWorldSpace(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::vec3& color);
    
    // Set attenuation parameters
    void SetAttenuation(float c, float l, float q);

    // Override entire model color (ignores texture if enabled)
    void EnableCustomColor(bool enable);
    void SetCustomColor(const glm::vec3& color);
};