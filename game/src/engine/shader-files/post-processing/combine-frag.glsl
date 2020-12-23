#version 330 core

uniform sampler2D uColorTexture;
uniform sampler2D uHighlightTexture;

in VertexData {
	vec2 textureCoords;
} f_in;

out vec4 outColor;

void main()
{
	vec4 sceneColor = texture(uColorTexture, f_in.textureCoords);
	vec4 highlightColor = texture(uHighlightTexture, f_in.textureCoords);
	outColor = sceneColor + highlightColor;
	//outColor *= vec4(0.776, 0.780, 0.941, 1);
}
