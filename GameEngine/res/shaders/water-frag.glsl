#version 330 core

uniform sampler2D uReflection;
uniform sampler2D uRefraction;
uniform sampler2D uDistortionMap;
uniform sampler2D uNormalMap;
uniform float uMoveFactor; // simulate moving water ripples

uniform vec3 uLightColor;
uniform vec3 uLightPosition;

in VertexData {
    vec3 position;
    vec2 textureCoords;
    vec4 clipSpace;
    vec3 toCamera;
} f_in;

const float distortionStrength = 0.010;
const float shineDamper = 20.0;
const float reflectivity = 0.5;

out vec4 outColor;

void main() {
    vec2 ndcCoords = vec2(f_in.clipSpace) / f_in.clipSpace.w;
    vec2 uvCoords = (ndcCoords + vec2(1.0)) / 2.0;

	vec2 distortedTexCoords = texture(uDistortionMap, vec2(f_in.textureCoords.x + uMoveFactor, f_in.textureCoords.y)).rg * 0.1;
	distortedTexCoords = f_in.textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + uMoveFactor);
	vec2 totalDistortion = (texture(uDistortionMap, distortedTexCoords).rg * 2.0 - 1.0) * distortionStrength;

    vec2 reflectionCoords = vec2(uvCoords.x, -uvCoords.y) + totalDistortion;
    reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
    reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

    vec2 refractionCoords = vec2(uvCoords.x, uvCoords.y) + totalDistortion;
    refractionCoords = clamp(refractionCoords, 0.001, 0.999);

    vec4 reflectionColor = texture(uReflection, reflectionCoords);
    vec4 refractionColor = texture(uRefraction, refractionCoords);

    // Fresnel effect
    vec3 toCamera = normalize(f_in.toCamera);
    float fresnel = dot(toCamera, vec3(0.0, 1.0, 0.0));

    vec4 finalColor = mix(reflectionColor, refractionColor, fresnel); // blue

    // Specular highlights using normal mapping
    vec4 normalColor = texture(uNormalMap, distortedTexCoords);
    // extract normal from colours
    vec3 normal = normalize(vec3(normalColor.r * 2.0 - 1.0, normalColor.b, normalColor.g * 2.0 - 1.0));
    vec3 lightToPoint = f_in.position - uLightPosition;
    vec3 incidentLight = normalize(lightToPoint);
    vec3 reflectDir = reflect(incidentLight, normal);
    float spec = pow(max(dot(toCamera, reflectDir), 0.0), shineDamper);
    vec3 specular = reflectivity * spec * uLightColor;

    // tint slightly
    finalColor = mix(finalColor, vec4(0, 0.1, 0.1, 1), 0.2) + vec4(specular, 0.0);
    outColor = finalColor;
}