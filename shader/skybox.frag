#version 330 compatibility

out vec4 FragColor;
in vec3 WorldPos;

uniform sampler2D equirectangularMap;

// Mathematical constants for spherical mapping
const vec2 invAtan = vec2(0.1591, 0.3183); // 1/(2*PI), 1/PI

// Function to convert 3D direction vector to 2D UV coordinates
vec2 SampleSphericalMap(vec3 v) {
    // Calculate longitude and latitude using arctangent and arcsine
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    // Normalize the interpolated 3D position to get a pure direction vector
    vec2 uv = SampleSphericalMap(normalize(WorldPos));
    
    // Sample the 2D texture using the calculated spherical UVs
    vec3 color = texture(equirectangularMap, uv).rgb;
    
    FragColor = vec4(color, 1.0);
}