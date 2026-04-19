#version 330 compatibility

out vec4 FragColor;

in vec3 Normal;
in vec3 ourColor;
in vec2 TexCoord;

uniform bool hasTexture;
uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    vec4 baseColor = (hasTexture ? texColor : vec4(1.0)) * vec4(ourColor, 1.0);
    vec3 norm = normalize(Normal);

    vec3 keyLightDir = normalize(vec3(1.5, 1.0, 0.8));
    float keyIntensity = max(dot(norm, keyLightDir), 0.0);

    vec3 fillLightDir = normalize(vec3(-0.5, 1.0, -0.3));
    float fillIntensity = max(dot(norm, fillLightDir), 0.0) * 0.5;

    vec3 viewDir = vec3(0.0, 0.0, 1.0);
    float rimWeight = 1.0 - max(dot(norm, viewDir), 0.0);
    float rimLight = smoothstep(0.0, 1.0, rimWeight) * 0.5;

    vec3 ambientLight = vec3(0.1);

    vec3 finalColor = ambientLight + vec3(keyIntensity * 0.4) + vec3(fillIntensity) + vec3(rimLight);
    
    FragColor = vec4(baseColor.rgb * finalColor, baseColor.a);
}