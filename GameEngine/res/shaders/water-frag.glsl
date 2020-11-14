#version 330 core

in VertexData {
    vec2 textureCoords;
} f_in;

out vec4 outColor;

void main() {
    outColor = vec4(0.0, 0.7, 0.9, 1.0); // blue
}