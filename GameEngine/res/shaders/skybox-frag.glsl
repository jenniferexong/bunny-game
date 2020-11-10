#version 330 core

uniform samplerCube cubeMap;

in VertexData {
    vec3 textureCoords;
} f_in;

out vec4 outColor;

void main() {
    outColor = texture(cubeMap, f_in.textureCoords);
}