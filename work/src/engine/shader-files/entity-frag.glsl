#version 330 core

// Uniform data
uniform sampler2D uDiffuseMap;
uniform sampler2D uGlowMap;

uniform vec3 uLightColor[100];
uniform vec3 uLightPosition[100];
uniform vec3 uAttenuation[100];

uniform int uLightCount;
uniform int uMaxLights;

uniform float uReflectivity;
uniform float uShineDamper;
uniform vec3 uFogColor;
uniform vec3 uSunStrength;

in VertexData {
    vec3 position;
    vec3 normal;
    vec2 textureCoords;
    vec3 cameraPosition;
    float modelBrightness;
    float visibility;
} f_in; 

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outGlow;

void main() 
{
    // Phong shading
    vec4 modelColor = texture(uDiffuseMap, f_in.textureCoords);
    if (modelColor.a < 0.5)
        discard;

    vec3 toCamera = normalize(f_in.cameraPosition - f_in.position);
    vec3 norm = normalize(f_in.normal);

    float ambientStrength = f_in.modelBrightness;
	vec3 ambient = ambientStrength * uSunStrength;
	ambient = f_in.modelBrightness > 1 ? vec3(1) : vec3(0.2);

    // Lighting calculations
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    for (int i = 0; i < uLightCount && i < uMaxLights; i++) {
        vec3 lightToPoint = f_in.position - uLightPosition[i];
        vec3 incidentLight = normalize(lightToPoint);

        float dist = length(lightToPoint);
        float attenuationFactor = (
			uAttenuation[i].x 
			+ (uAttenuation[i].y * dist) 
			+ (uAttenuation[i].z * dist * dist)
		);
        
        float diff = max(dot(norm, -incidentLight), 0.0);
        diffuse += (diff * uLightColor[i]) / attenuationFactor;

        vec3 reflectDir = reflect(incidentLight, norm);
        float spec = pow(max(dot(toCamera, reflectDir), 0.0), uShineDamper);
        specular += (uReflectivity * spec * uLightColor[i]) / attenuationFactor;
    }

	// calculating the glow colour
	outGlow = vec4(0);
	float glow = texture(uGlowMap, f_in.textureCoords).r;
	if (glow > 0) {
		diffuse += vec3(glow);
		outGlow = modelColor * vec4(diffuse, 1);
	}

    //vec3 fogColor = uSunStrength * uFogColor;
    vec3 result = vec3(vec4(ambient + diffuse + specular, 1) * modelColor);
    outColor = vec4(result, 1.0);
}
