#version 330 core

uniform sampler2D uReflection;
uniform sampler2D uRefraction;

in VertexData {
    vec4 clipSpace;
} f_in;

out vec4 outColor;

void main() {
    vec2 ndcCoords = vec2(f_in.clipSpace) / f_in.clipSpace.w;
    vec2 uvCoords = (ndcCoords + vec2(1.0, 1.0)) / 2.0;

    vec2 reflectionCoords = vec2(uvCoords.x, -uvCoords.y);
    vec2 refractionCoords = vec2(uvCoords.x, uvCoords.y);

    vec4 reflectionColor = texture(uReflection, reflectionCoords);
    vec4 refractionColor = texture(uRefraction, refractionCoords);

    outColor = mix(reflectionColor, refractionColor, 0.5); // blue
}