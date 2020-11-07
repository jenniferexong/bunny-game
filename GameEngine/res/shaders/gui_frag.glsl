#version 330 core

uniform sampler2D textureSampler;

in VertexData {
    vec2 textureCoords;
} f_in; 

out vec4 outColor;

void main(void){
	outColor = texture(textureSampler, f_in.textureCoords);
}