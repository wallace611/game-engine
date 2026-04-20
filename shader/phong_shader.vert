#version 330 compatibility

// Vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

// Outputs to the fragment shader (All in World Space)
out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
out vec2 TexCoords;

// Model matrix passed from C++ (parent->GetGlobalMatrix())
uniform mat4 modelMatrix;

void main() {
    // 1. Final screen projection using built-in fixed pipeline matrix
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1.0);
    
    // 2. Calculate fragment position in WORLD SPACE using the uniform modelMatrix
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    
    // 3. Calculate normal in WORLD SPACE
    // Using transpose and inverse ensures correct normals even if the model is non-uniformly scaled
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;

    // 4. Pass color and texture coordinates unmodified
    Color = aColor;
    TexCoords = aTexCoords;
}