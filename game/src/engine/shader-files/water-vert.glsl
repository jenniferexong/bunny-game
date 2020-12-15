#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec3 uCameraPosition;

out VertexData {
    vec3 position;
    vec2 textureCoords;
    vec4 clipSpace;
    vec3 toCamera;
} v_out;

const float tiling = 1;

void main() 
{
    vec4 worldPosition =
		uTransformationMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0);

    v_out.position = vec3(worldPosition);
    v_out.clipSpace = uProjectionMatrix * uViewMatrix * worldPosition;
    gl_Position = v_out.clipSpace;

    v_out.textureCoords = 
		vec2(aPosition.x / 2.0 + 0.5, aPosition.y / 2.0 + 0.5) * tiling;

    v_out.toCamera = uCameraPosition - vec3(worldPosition);
}
