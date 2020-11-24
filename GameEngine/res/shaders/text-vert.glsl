#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTextureCoords;

uniform vec2 uTranslation;

out VertexData {
    vec2 textureCoords;
} v_out;

void main() {
    vec2 translation = vec2(uTranslation.x * 2.0, uTranslation.y * -2.0);
    gl_Position = vec4(aPosition + translation, 0.0, 1.0);
    v_out.textureCoords = aTextureCoords;
}
