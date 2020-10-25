#version 330 core

uniform sampler2D textureSampler;

in VertexData {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textureCoords;
} f_in; 

out vec4 outColor;

void main() {
    // Mix the colour and the texture
    outColor = vec4(f_in.color, 1.0) * texture(textureSampler, f_in.textureCoords);

    // Phong shading
}