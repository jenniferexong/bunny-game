#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

out VertexData {
    vec2 textureCoords;
} v_out;

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * uTransformationMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0);
	v_out.textureCoords = vec2(aPosition.x / 2.0 + 0.5, aPosition.y / 2.0 + 0.5);
}