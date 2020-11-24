#version 330 core

uniform vec3 uColor;
uniform sampler2D textureAtlas;

in VertexData {
    vec2 textureCoords;
} f_in;

out vec4 outColor;

void main() {
    outColor = vec4(uColor, texture(textureAtlas, f_in.textureCoords).a);
    //outColor = vec4(texture(textureAtlas, f_in.textureCoords).xyz, 1.0);
}