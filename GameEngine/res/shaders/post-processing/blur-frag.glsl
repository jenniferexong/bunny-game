#version 330 core

uniform sampler2D uOriginalTexture;

out vec4 outColor;

in VertexData {
    vec2 textureCoordsBlur[11];
} f_in;

const float weights[11] = { 0.0093,	0.028002, 0.065984,	0.121703, 0.175713, 0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093 };

void main() {

    outColor = vec4(0.0);
    for (int i = 0; i < 11; i++) {
        outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[i]) * weights[i];
    }
    /*
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[0]) * 0.0093;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[1]) * 0.028002;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[2]) * 0.065984;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[3]) * 0.121703;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[4]) * 0.175713;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[5]) * 0.198596;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[6]) * 0.175713;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[7]) * 0.121703;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[8]) * 0.065984;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[9]) * 0.028002;
    outColor += texture(uOriginalTexture, f_in.textureCoordsBlur[10]) * 0.0093;
    */
}