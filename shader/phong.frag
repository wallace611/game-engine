#version 330 compatibility

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() {
    // 1. Calculate the distance between the light and the current pixel
    float distance = length(lightPos - FragPos);
    
    // 2. Calculate Attenuation (Standard values for a light covering ~50 distance units)
    // Constant (Kc) = 1.0, Linear (Kl) = 0.09, Quadratic (Kq) = 0.032
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

    // 3. Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // 4. Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // 5. Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;  
        
    // 6. Apply attenuation to all lighting components (Ambient can be excluded if desired)
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // 7. Combine lighting and texture
    vec3 lighting = ambient + diffuse + specular;
    vec4 texColor = texture(texture1, TexCoord);
    vec3 baseColor = texColor.rgb * ourColor;

    vec3 result = lighting * baseColor;
    FragColor = vec4(result, texColor.a);
}