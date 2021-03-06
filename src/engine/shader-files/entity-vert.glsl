#version 330 core 

// Uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uInverseViewMatrix;
uniform int uFakeLighting;

uniform vec4 uClippingPlane;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoords;
layout(location = 3) in mat4 aModelMatrix;
layout(location = 7) in float aModelBrightness;

out VertexData {
    vec3 position;
    vec3 normal;
    vec2 textureCoords;
    vec3 cameraPosition;
    float modelBrightness;
    float visibility;
	vec3 reflectedVector;
	vec3 refractedVector;
} v_out; 

const float density = 0.005;
const float fogGradient = 2.0;

void main() 
{
    // vertex position in world coordinates 
	// (only multiplied by model transformation)
    vec4 worldPosition = aModelMatrix * vec4(aPosition, 1); 
    vec4 positionRelativeToCamera = uViewMatrix * worldPosition;

    // get the signed distance between the 
	// vertex position and the clipping plane
    gl_ClipDistance[0] = dot(worldPosition, uClippingPlane);

    v_out.position = vec3(worldPosition);

    gl_Position = uProjectionMatrix * positionRelativeToCamera;

    //v_out.normal = mat3(transpose(inverse(uTransformationMatrix))) * aNormal;
    vec3 actualNormal = uFakeLighting == 1 ? vec3(0.0, 1.0, 0.0) : aNormal;
    v_out.normal = vec3(aModelMatrix * vec4(actualNormal, 0));
    v_out.cameraPosition = vec3(uInverseViewMatrix * vec4(0, 0, 0, 1));

    v_out.textureCoords = aTextureCoords;
    v_out.modelBrightness = aModelBrightness;

	// reflection, refraction
	vec3 viewVector = normalize(vec3(worldPosition) - v_out.cameraPosition);
	v_out.reflectedVector = reflect(viewVector, normalize(v_out.normal));
	v_out.refractedVector = refract(
		viewVector,
		normalize(v_out.normal),
		1.0/1.33
	);

    // fog calculations 
    // float distance = length(positionRelativeToCamera.xyz); 
    //v_out.visibility = 
	//clamp(exp(-pow(distance * density, fogGradient)), 0.0, 1.0);
}
