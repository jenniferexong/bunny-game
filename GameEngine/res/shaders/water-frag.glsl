#version 330 core

uniform sampler2D uReflection;
uniform sampler2D uRefraction;
uniform sampler2D uDistortionMap;
uniform sampler2D uNormalMap;
uniform sampler2D uDepthMap;

uniform float uMoveFactor; // simulate moving water ripples

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

const float distortionStrength = 0.02;
const float shineDamper = 100.0;
const float reflectivity = 0.6;

out vec4 outColor;

void main() {
    vec2 ndcCoords = vec2(f_in.clipSpace) / f_in.clipSpace.w;
    vec2 uvCoords = (ndcCoords + vec2(1.0)) / 2.0;

    vec2 reflectionCoords = vec2(uvCoords.x, -uvCoords.y);
    vec2 refractionCoords = vec2(uvCoords.x, uvCoords.y);

    // depth calculations
    float depth = texture(uDepthMap, refractionCoords).r;
    // distance from the camera to the terrain under the water
    float floorDistance = 2.0 * uNearPlane * uFarPlane / (uFarPlane + uNearPlane - (depth * 2.0 - 1.0) * (uFarPlane - uNearPlane));
    depth = gl_FragCoord.z;
    // distance from the camera to the water fragment
    float waterDistance = 2.0 * uNearPlane * uFarPlane / (uFarPlane + uNearPlane - (depth * 2.0 - 1.0) * (uFarPlane - uNearPlane));
    float waterDepth = floorDistance - waterDistance;

    // water distortions
	vec2 distortedTexCoords = texture(uDistortionMap, vec2(f_in.textureCoords.x + uMoveFactor, f_in.textureCoords.y)).rg * 0.1;
	distortedTexCoords = f_in.textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + uMoveFactor);
	vec2 totalDistortion = (texture(uDistortionMap, distortedTexCoords).rg * 3.0 - 1.0) * distortionStrength;
    totalDistortion *= clamp(waterDepth/20.0, 0.0, 1.0); // less distortion if water depth is < 20

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
    vec3 normal = normalize(vec3(normalColor.r * 2.0 - 1.0, normalColor.b * 5.0, normalColor.g * 2.0 - 1.0));

    // Fresnel effect
    vec3 toCamera = normalize(f_in.toCamera);
    float fresnel = dot(toCamera, normal);

    vec4 finalColor = mix(reflectionColor, refractionColor, fresnel); // blue

    // Specular highlights using normal mapping
    vec3 lightToPoint = f_in.position - uLightPosition;
    vec3 incidentLight = normalize(lightToPoint);
    vec3 reflectDir = reflect(incidentLight, normal);
    float spec = pow(max(dot(toCamera, reflectDir), 0.0), shineDamper);
    vec3 specular = reflectivity * spec * uLightColor;
    specular *= clamp(waterDepth/15.0, 0.0, 1.0); // dampen specular highlights when water less deep

    // tint slightly
    finalColor = mix(finalColor, vec4(0, 0.1, 0.1, 1), 0.2) + vec4(specular, 0.0);
    finalColor.a = clamp(waterDepth/2.0, 0.0, 1.0);
    outColor = finalColor;
}