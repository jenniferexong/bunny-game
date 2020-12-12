#version 330 core

layout(location = 0) in vec2 aPosition;

uniform float uTargetWidth;

out VertexData {
    vec2 textureCoordsBlur[11];
} v_out;

void main() {
    gl_Position = vec4(aPosition, 0.0, 1.0);
    vec2 textureCoordsCenter = (aPosition + 1.0) / 2.0;
    float pixelSize = 1.0 / uTargetWidth;

    for (int i = -5; i <= 5; i++) {
        v_out.textureCoordsBlur[i + 5] = textureCoordsCenter + vec2(pixelSize * i, 0.0);
    }
}