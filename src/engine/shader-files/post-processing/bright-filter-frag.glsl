#version 330 core

uniform sampler2D colorTexture;

in VertexData {
	vec2 textureCoords;
} f_in;

out vec4 outColor;

void main() 
{
	vec4 color = texture(colorTexture, f_in.textureCoords);
	// luma conversion
	float brightness = (
		(color.r * 0.2126)
		+ (color.g * 0.7152)
		+ (color.b * 0.0722)
	);

	outColor = color * brightness * brightness;
	//outColor = brightness > 0.7 ? color : vec4(0);
}

