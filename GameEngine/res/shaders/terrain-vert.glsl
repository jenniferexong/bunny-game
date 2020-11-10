#version 330 core 

// Uniform data
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uInverseViewMatrix;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoords;

out VertexData {
    vec3 position;
    vec3 normal;
    vec2 textureCoords;
    vec3 cameraPosition;
    float visibility;
} v_out; 

const float density = 0.005;
const float fogGradient = 2.5;

void main() {
    vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1); 
    vec4 positionRelativeToCamera = uViewMatrix * worldPosition;

    // vertex position in world coordinates (only multiplied by model transformation)
    v_out.position = vec3(worldPosition);
    gl_Position = uProjectionMatrix * positionRelativeToCamera;

    //v_out.normal = mat3(transpose(inverse(uTransformationMatrix))) * aNormal;
    v_out.normal = vec3(uTransformationMatrix * vec4(aNormal, 0));
    v_out.cameraPosition = vec3(uInverseViewMatrix * vec4(0, 0, 0, 1));

    v_out.textureCoords = aTextureCoords;

    // fog calculations
    float distance = length(positionRelativeToCamera.xyz);
    v_out.visibility = clamp(exp(-pow(distance * density, fogGradient)), 0.0, 1.0);
}