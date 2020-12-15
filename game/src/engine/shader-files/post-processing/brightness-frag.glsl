#version 330 core

uniform sampler2D uColorTexture;
uniform float uBrightness;

in VertexData {
    vec2 textureCoords;
} f_in;

out vec4 outColor;

void main() 
{
    outColor = texture(uColorTexture, f_in.textureCoords);
	outColor += vec4((uBrightness * outColor).rgb, 0.0);
	//outColor += uBrightness * vec4(uBrightness, uBrightness, uBrightness, 0.0);
}
