#version 330 core

uniform vec3 uColor;
uniform sampler2D textureAtlas;
uniform float uCharacterWidth;
uniform float uEdgeTransition;

in VertexData {
    vec2 textureCoords;
} f_in;

out vec4 outColor;

void main() {
    float distance = 1.0 - texture(textureAtlas, f_in.textureCoords).a; // get distance information

    // get alpha based on distance with respect to two edges
    float alpha = 1.0 - smoothstep(uCharacterWidth, uCharacterWidth + uEdgeTransition, distance);

    outColor = vec4(uColor, alpha);
}