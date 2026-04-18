#version 330 compatibility

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform bool hasTexture;

uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    
    FragColor = (hasTexture ? texColor : vec4(1.0)) * vec4(ourColor, 1.0);
}