#version 330 core

uniform samplerCube uSkyTexture;
uniform samplerCube uStarsTexture;

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
    vec4 finalColor = texture(uSkyTexture, f_in.textureCoords);

    // check if the point in the sky is below the lowerLimit or above the upperLimit
    float factor = (f_in.textureCoords.y - lowerLimit) / (upperLimit - lowerLimit);

    // 0 means use fog colour, 1 means use texture colour
    factor = clamp(factor, 0.0, 1.0);
    finalColor = vec4(uSunStrength, 1.0) * mix(vec4(uFogColor, 1.0), finalColor, factor);

    // add stars texture, amount based on how strong the current sunlight is (low sunlight means brighter stars)
    vec4 stars = texture(uStarsTexture, f_in.textureCoords);
    float starsFactor = max((0.5 - uSunStrength.x), 0.0);
    finalColor += starsFactor * stars;
    outColor = finalColor;
}