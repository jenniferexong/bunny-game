#version 330 core

uniform samplerCube cubeMap;
uniform vec3 uFogColor;
uniform vec3 uSunStrength;

in VertexData {
    vec3 textureCoords;
} f_in;

// For blending the sky into the fog colour
const float lowerLimit = -10.0;
const float upperLimit = 40.0;

out vec4 outColor;

void main() {
    vec4 finalColor = texture(cubeMap, f_in.textureCoords);

    // check if the point in the sky is below the lowerLimit or above the upperLimit
    float factor = (f_in.textureCoords.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0, 1.0);
    outColor = vec4(uSunStrength, 1.0) * mix(vec4(uFogColor, 1.0), finalColor, factor);
}