#version 330 core 

// Uniform data
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

out VertexData {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textureCoords;
} v_out; 

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * uTransformationMatrix * vec4(position, 1);
    //v_out.color = vec3(position.x + 0.5, 0.0, position.y + 0.5);
    v_out.normal = normal;
    v_out.textureCoords = textureCoords;
}