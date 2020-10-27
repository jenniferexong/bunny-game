#version 330 core 

// Uniform data
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoords;

out VertexData {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textureCoords;
} v_out; 

void main() {
    gl_Position = uProjectionMatrix * uViewMatrix * uTransformationMatrix * vec4(aPosition, 1);

    v_out.position = vec3(uTransformationMatrix * vec4(aPosition,1));

    //v_out.normal = mat3(transpose(inverse(uTransformationMatrix))) * aNormal;
    v_out.normal = vec3(uTransformationMatrix * vec4(aNormal, 0));
    v_out.color = vec3(0, 0.7, 0.7);

    v_out.textureCoords = aTextureCoords;
}