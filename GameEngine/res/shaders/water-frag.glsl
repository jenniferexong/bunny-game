#version 330 core

uniform sampler2D uReflection;
uniform sampler2D uRefraction;
uniform sampler2D uDistortionMap;

uniform float uMoveFactor; // simulate moving water ripples

in VertexData {
    vec2 textureCoords;
    vec4 clipSpace;
} f_in;

const float distortionStrength = 0.03;

out vec4 outColor;

void main() {
    vec2 ndcCoords = vec2(f_in.clipSpace) / f_in.clipSpace.w;
    vec2 uvCoords = (ndcCoords + vec2(1.0)) / 2.0;

    vec2 distortionCoord = vec2(f_in.textureCoords.x + uMoveFactor, f_in.textureCoords.y);
    vec2 distortion1 = texture(uDistortionMap, distortionCoord).rg;
    distortion1 = (distortion1 * 2.0) - vec2(1.0); // convert to range [-1, 1]
    distortion1 *= distortionStrength;

    distortionCoord = vec2(-f_in.textureCoords.x + uMoveFactor, f_in.textureCoords.y + uMoveFactor);
    vec2 distortion2 = texture(uDistortionMap, distortionCoord).rg;
    distortion2 = (distortion2 * 2.0) - vec2(1.0); // convert to range [-1, 1]
    distortion2 *= distortionStrength;

    vec2 totalDistortion = distortion1 + distortion2;

    vec2 reflectionCoords = vec2(uvCoords.x, -uvCoords.y) + totalDistortion;
    reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
    reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

    vec2 refractionCoords = vec2(uvCoords.x, uvCoords.y) + totalDistortion;
    refractionCoords = clamp(refractionCoords, 0.001, 0.999);

    vec4 reflectionColor = texture(uReflection, reflectionCoords);
    vec4 refractionColor = texture(uRefraction, refractionCoords);

    vec4 finalColor = mix(reflectionColor, refractionColor, 0.5); // blue

    // tint slightly
    finalColor = mix(finalColor, vec4(0, 0.1, 0.1, 1), 0.2);
    outColor = finalColor;
}