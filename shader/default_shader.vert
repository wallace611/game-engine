#version 330 compatibility

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 ourColor;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1.0);
    
    Normal = gl_NormalMatrix * aNormal;
    ourColor = aColor;
    TexCoord = aTexCoord;
}