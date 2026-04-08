#version 330 compatibility

// Input vertex attributes matching the C++ layout locations
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

// Output variables to pass data to the Fragment Shader
out vec3 ourColor;
out vec2 TexCoord;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1.0);
    
    // Pass the color and UV data to the fragment shader
    ourColor = aColor;
    TexCoord = aTexCoord;
}