#version 330 core

uniform sampler2D uColorTexture;

in VertexData {
    vec2 textureCoords;
} f_in;

out vec4 outColor;
const float contrast = 0.05;

void main() 
{
    outColor = texture(uColorTexture, f_in.textureCoords);
    outColor.rgb = (outColor.rgb - 0.5) * (1.0 + contrast) + 0.5;
}
