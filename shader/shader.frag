#version 330 compatibility

// Output color of the pixel
out vec4 FragColor;

// Variables interpolated and passed from the Vertex Shader
in vec3 ourColor;
in vec2 TexCoord;

// The texture sampler bound by C++
uniform sampler2D texture1;

void main() {
    // Sample the texture color at the given UV coordinate
    vec4 texColor = texture(texture1, TexCoord);
    
    // Multiply the texture color with the vertex color for a tinted effect
    FragColor = texColor * vec4(ourColor, 1.0);
}