#version 330 compatibility

out vec4 FragColor;

// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoords;

// Texture uniforms
uniform sampler2D texture_diffuse1;
uniform bool hasTexture;

// Light uniforms (Position is in View Space)
uniform vec3 lightPosViewSpace;
uniform vec3 lightColor;

// Attenuation uniforms
uniform float constant;
uniform float linear;
uniform float quadratic;

// Override color uniforms
uniform bool useCustomColor;
uniform vec3 customColor;

void main() {
    // 1. Resolve Base Color (Override logic)
    vec4 baseColor = vec4(1.0);
    if (useCustomColor) {
        // Completely override texture and model color
        baseColor = vec4(customColor, 1.0);
    } else if (hasTexture) {
        // Fallback to texture if no custom color is used
        baseColor = texture(texture_diffuse1, TexCoords);
    } else {
        baseColor = vec4(Color, 1.0f);
    }

    // 2. Setup standard Phong variables (View Space)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosViewSpace - FragPos);

    // Ambient Lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse Lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular Lighting
    float specularStrength = 0.5;
    // In View Space, the camera is strictly at the origin (0, 0, 0)
    vec3 viewDir = normalize(-FragPos); 
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    // 3. Attenuation (Falloff calculation)
    float distance = length(lightPosViewSpace - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Apply distance attenuation to all light components
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // 4. Final Color Composition
    vec3 lightingResult = ambient + diffuse + specular;
    FragColor = vec4(lightingResult * baseColor.rgb, baseColor.a);
}