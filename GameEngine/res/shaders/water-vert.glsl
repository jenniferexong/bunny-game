#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

out VertexData {
    vec2 textureCoords;
    vec4 clipSpace;
} v_out;

const float tiling = 6.0;

void main() {
    v_out.clipSpace = uProjectionMatrix * uViewMatrix * uTransformationMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0);
    gl_Position = v_out.clipSpace;
    v_out.textureCoords = vec2(aPosition.x / 2.0 + 0.5, aPosition.y / 2.0 + 0.5) * tiling;
}