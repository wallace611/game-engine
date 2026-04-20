#version 330 compatibility

// Vertex attributes using explicit layout locations
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

// Outputs for the fragment shader (all in View Space)
out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
out vec2 TexCoords;

void main() {
    // Transform vertex position using the built-in fixed pipeline matrix
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1.0);
    
    // Calculate the fragment position and normal in View Space
    FragPos = vec3(gl_ModelViewMatrix * vec4(aPos, 1.0));
    Normal = gl_NormalMatrix * aNormal;

    Color = aColor;
    
    // Pass texture coordinates unmodified
    TexCoords = aTexCoords;
}