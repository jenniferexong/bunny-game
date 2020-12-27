#version 330 core 

// Uniform data
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uInverseViewMatrix;

uniform vec4 uClippingPlane;

uniform int uLightCount;
uniform int uMaxLights;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoords;
layout(location = 3) in vec3 aTangent;

out VertexData {
    vec2 textureCoords;
    vec3 eyeSpacePosition;
    mat3 toTangentSpace;
    float visibility;
} v_out; 

const float density = 0.005;
const float fogGradient = 2.0;

void main() 
{
    vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1); 
    vec4 positionRelativeToCamera = uViewMatrix * worldPosition;

    // get the signed distance between the 
	// vertex position and the clipping plane
    gl_ClipDistance[0] = dot(worldPosition, uClippingPlane);

    // vertex position in world coordinates 
	// (only multiplied by model transformation)
    gl_Position = uProjectionMatrix * positionRelativeToCamera;

    //v_out.normal = mat3(transpose(inverse(uTransformationMatrix))) * aNormal;

    // tangent space
    mat4 modelViewMatrix = uViewMatrix * uTransformationMatrix;
    vec3 surfaceNormal = vec3(modelViewMatrix * vec4(aNormal, 0));

    vec3 normal = normalize(surfaceNormal);
    vec3 tangent = normalize(vec3(modelViewMatrix * vec4(aTangent, 0)));
    vec3 biTangent = normalize(cross(normal, tangent));

    mat3 toTangentSpace = mat3(
        tangent.x, biTangent.x, normal.x,
        tangent.y, biTangent.y, normal.y,
        tangent.z, biTangent.z, normal.z
    );

    v_out.toTangentSpace = toTangentSpace;
    v_out.eyeSpacePosition = positionRelativeToCamera.xyz;

    v_out.textureCoords = aTextureCoords;

    // fog calculations
    float distance = length(positionRelativeToCamera.xyz);
    v_out.visibility = clamp(
		exp(-pow(distance * density, fogGradient)), 0.0, 1.0
	);
}
