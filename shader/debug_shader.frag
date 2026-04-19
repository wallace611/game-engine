#version 330 compatibility

uniform vec4 debugColor;

out vec4 FragColor;

void main() {
    FragColor = debugColor;
}