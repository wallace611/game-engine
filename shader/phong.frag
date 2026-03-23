#version 330 compatibility

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 ourColor;
in vec2 TexCoord;

// We now have TWO texture samplers
uniform sampler2D texture1;
uniform sampler2D equirectangularMap; 

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

// Settings
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;
uniform vec2 tiling; 
uniform vec2 scrollSpeed;
uniform float time;
uniform float reflectivity; // NEW

// Math for converting 3D reflection vector to 2D Panorama UV
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    // 1. Standard Phong Lighting
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    // Note: reflect expects incident vector (FROM camera TO fragment)
    vec3 incidentDir = -viewDir; 
    vec3 reflectDir = reflect(incidentDir, norm);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;  
    
    ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;
    vec3 lighting = ambient + diffuse + specular;
    
    // 2. Base Texture
    vec2 currentUV = (TexCoord * tiling) + (scrollSpeed * time);
    vec4 texColor = texture(texture1, currentUV);
    vec3 baseColor = texColor.rgb * ourColor;
    vec3 finalColor = lighting * baseColor;

    // 3. Environment Reflection (Only apply if reflectivity > 0.0)
    if (reflectivity > 0.0) {
        // Calculate where the reflection vector hits the panorama map
        vec2 envUV = SampleSphericalMap(normalize(reflectDir));
        vec3 envColor = texture(equirectangularMap, envUV).rgb;
        
        // Blend the base rendered color with the environment reflection
        finalColor = mix(finalColor, envColor, reflectivity);
    }
    
    FragColor = vec4(finalColor, texColor.a);
}