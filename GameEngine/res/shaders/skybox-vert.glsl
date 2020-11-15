#version 330 core

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec4 uClippingPlane;

layout(location = 0) in vec3 aPosition;

out VertexData {
    vec3 textureCoords;
} v_out;

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * vec4(aPosition, 1.0);
    v_out.textureCoords = aPosition;
}