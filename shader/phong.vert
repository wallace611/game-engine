#version 330 compatibility

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec3 ourColor;
out vec2 TexCoord;

// The model matrix sent from C++
uniform mat4 model;

void main() {
    // Calculate fragment position in World Space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Correctly scale/rotate the normal vector using the normal matrix
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    ourColor = aColor;
    TexCoord = aTexCoord;

    // Apply the legacy camera projection & view, multiplied by our modern model matrix
    gl_Position = gl_ModelViewProjectionMatrix * model * vec4(aPos, 1.0);
}