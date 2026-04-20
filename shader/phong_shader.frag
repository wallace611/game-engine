#version 330 compatibility

out vec4 FragColor;

// Inputs from vertex shader (World Space)
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoords;

// Texture uniforms
uniform sampler2D texture_diffuse1;
uniform bool hasTexture;

// Light and Camera uniforms (World Space - matching C++ uniforms)
uniform vec3 lightPosWorld;
uniform vec3 lightColor;
uniform vec3 viewPosWorld;

// Attenuation uniforms
uniform float constant;
uniform float linear;
uniform float quadratic;

// Override color uniforms
uniform bool useCustomColor;
uniform vec3 customColor;

void main() {
    // 1. Resolve Base Color
    vec4 baseColor = vec4(1.0);
    if (useCustomColor) {
        // High priority: completely override texture and vertex color
        baseColor = vec4(customColor, 1.0);
    } else if (hasTexture) {
        // Medium priority: use texture
        baseColor = texture(texture_diffuse1, TexCoords);
    } else {
        // Low priority: fallback to vertex color
        baseColor = vec4(Color, 1.0);
    }

    // 2. Setup standard Phong variables (World Space)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosWorld - FragPos);

    // Ambient Lighting
    float ambientStrength = 0.6;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse Lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular Lighting
    float specularStrength = 0.5;
    // Calculate view direction using the camera's global position
    vec3 viewDir = normalize(viewPosWorld - FragPos); 
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // 32.0 is the shininess factor
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    // 3. Attenuation (Falloff calculation in World Space)
    float distance = length(lightPosWorld - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Apply distance attenuation to all light components
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // 4. Final Color Composition
    vec3 lightingResult = ambient + diffuse + specular;
    FragColor = vec4(lightingResult * baseColor.rgb, baseColor.a);
}