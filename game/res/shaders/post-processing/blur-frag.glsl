#version 330 core

uniform sampler2D uOriginalTexture;

out vec4 outColor;

in VertexData {
    vec2 textureCoordsBlur[11];
} f_in;

const float weights[11] = float[](
	0.0093, 0.028002, 0.065984, 0.121703, 0.175713, 0.198596,
	0.175713, 0.121703, 0.065984, 0.028002, 0.0093
);

void main() 
{
    outColor = vec4(0.0);
    for (int i = 0; i < 11; i++) {
        outColor += 
			texture(uOriginalTexture, f_in.textureCoordsBlur[i]) * weights[i];
	}
}
