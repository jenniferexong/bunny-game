#version 330 core

uniform sampler2D uReflection;
uniform sampler2D uRefraction;
uniform sampler2D uDistortionMap;
uniform sampler2D uNormalMap;
uniform sampler2D uDepthMap;

// simulate moving water ripples
uniform float uMoveFactor; 

uniform vec3 uLightColor;
uniform vec3 uLightPosition;

uniform float uNearPlane;
uniform float uFarPlane;

in VertexData {
    vec3 position;
    vec2 textureCoords;
    vec4 clipSpace;
    vec3 toCamera;
} f_in;

const float distortionStrength = 0.03;
const float shineDamper = 100.0;
const float reflectivity = 0.6;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outGlow;

void main() 
{
    vec2 ndcCoords = vec2(f_in.clipSpace) / f_in.clipSpace.w;
    vec2 uvCoords = (ndcCoords + vec2(1.0)) / 2.0;

    vec2 reflectionCoords = vec2(uvCoords.x, -uvCoords.y);
    vec2 refractionCoords = vec2(uvCoords.x, uvCoords.y);

    // depth calculations
    float depth = texture(uDepthMap, refractionCoords).r;
    // distance from the camera to the terrain under the water
    float floorDistance = (
		2.0 * uNearPlane * uFarPlane 
		/ (uFarPlane + uNearPlane - (depth * 2.0 - 1.0) 
		* (uFarPlane - uNearPlane))
	 );
    depth = gl_FragCoord.z;
    // distance from the camera to the water fragment
    float waterDistance = (
		2.0 * uNearPlane * uFarPlane 
		/ (uFarPlane + uNearPlane - (depth * 2.0 - 1.0) 
		* (uFarPlane - uNearPlane))
	);
    float waterDepth = floorDistance - waterDistance;

    // water distortions
	vec2 distortedTexCoords = (
		texture(
			uDistortionMap,
			vec2(f_in.textureCoords.x + uMoveFactor, f_in.textureCoords.y)).rg 
		* distortionStrength * 2
	);
	distortedTexCoords = (
		f_in.textureCoords 
		+ vec2(distortedTexCoords.x, distortedTexCoords.y + uMoveFactor)
	);
	vec2 totalDistortion = (
		(texture(uDistortionMap, distortedTexCoords).rg * 3.0 - 1.0) 
		* distortionStrength
	);
	// less distortion if water depth is < 20
    totalDistortion *= clamp(waterDepth/40.0, 0.0, 1.0); 

    reflectionCoords += totalDistortion;
    refractionCoords += totalDistortion;
	refractionCoords = clamp(refractionCoords, 0.001, 0.999);
    reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
    reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

    vec4 reflectionColor = texture(uReflection, reflectionCoords);
    vec4 refractionColor = texture(uRefraction, refractionCoords);
    
    // normal mapping
    vec4 normalColor = texture(uNormalMap, distortedTexCoords);
    // extract normal from colours
    vec3 normal = normalize(
		vec3(
			normalColor.r * 2.0 - 1.0,
			normalColor.b * 5.0,
			normalColor.g * 2.0 - 1.0)
	);

    // Fresnel effect
    vec3 toCamera = normalize(f_in.toCamera);
    float fresnel = dot(toCamera, normal);
	fresnel = pow(fresnel, 0.5);

	// blue
    vec4 finalColor = mix(reflectionColor, refractionColor, fresnel); 

    // Specular highlights using normal mapping
    vec3 lightToPoint = f_in.position - uLightPosition;
    vec3 incidentLight = normalize(lightToPoint);
    vec3 reflectDir = reflect(incidentLight, normal);
    float spec = pow(max(dot(toCamera, reflectDir), 0.0), shineDamper);
    vec3 specular = reflectivity * spec * uLightColor;
	// dampen specular highlights when water is more shallow
    specular *= clamp(waterDepth/15.0, 0.0, 1.0); 

    // tint slightly
    finalColor = (
		mix(finalColor, vec4(0.0, 0.1, 0.1, 1), 0.1) 
		+ vec4(specular, 0.0)
	);
    finalColor.a = clamp(waterDepth/4.0, 0.1, 1.0);
    outColor = finalColor;
	outGlow = vec4(0.0);
}
