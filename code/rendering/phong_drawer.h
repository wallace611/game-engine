#pragma once

#include "rendering/model_drawer.h"
#include "object/object.h"

// Encapsulate all Phong material and light attenuation parameters
struct PhongSettings {
    // Light attenuation parameters (Default covers ~50 units)
    float constant = 1.0f;
    float linear = 0.07f;
    float quadratic = 0.0002f;

    // Material surface parameters
    float ambientStrength = 0.2f;
    float specularStrength = 0.5f;
    float shininess = 32.0f; // Higher means smaller, sharper specular highlight

    glm::vec2 tiling = glm::vec2(1.0f, 1.0f);
    glm::vec2 scrollSpeed = glm::vec2(0.0f, 0.0f);

    float reflectivity = 0.0f;
};

class PhongDrawer : public ModelDrawer {
private:
    Object* owner;
    PhongSettings settings; // Store the settings instance

    unsigned int envTexture;
    bool hasEnvMap;
public:
    PhongDrawer(const std::string& texture_path, Object* owner);
    
    virtual void Draw() override;

    // Setters and Getters for the material settings
    void SetSettings(const PhongSettings& newSettings);
    PhongSettings GetSettings() const;

    void SetEnvironmentMap(GLuint textureID);
};