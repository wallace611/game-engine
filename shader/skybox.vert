#version 330 compatibility

layout (location = 0) in vec3 aPos;

// Pass the local position directly as a 3D direction vector
out vec3 WorldPos;

uniform mat4 model;

void main() {
    WorldPos = aPos;
    // Standard projection
    gl_Position = gl_ModelViewProjectionMatrix * model * vec4(aPos, 1.0);
}