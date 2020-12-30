#version 330 core

uniform int uHasTexture;
uniform sampler2D uTexture;

uniform vec4 uColor;

in VertexData {
    vec2 textureCoords;
} f_in; 

out vec4 outColor;

void main(void)
{
	if (uHasTexture == 1)
		outColor = texture(uTexture, f_in.textureCoords);

	else
		outColor = uColor;
}
