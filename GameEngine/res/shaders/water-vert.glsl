#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

out VertexData {
    vec4 clipSpace;
} v_out;

void main() {
    v_out.clipSpace = uProjectionMatrix * uViewMatrix * uTransformationMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0);
    gl_Position = v_out.clipSpace;
}