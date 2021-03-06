#version 330 core

// Textures
uniform sampler2D uBaseTexture;
uniform sampler2D uRedTexture;
uniform sampler2D uGreenTexture;
uniform sampler2D uBlueTexture;
uniform sampler2D uBlendMap;

uniform sampler2D uShadowMap;

// normal map for path texture
uniform sampler2D uNormalMap; 

// positions in eye space
uniform vec3 uLightPosition[100]; 
uniform vec3 uLightColor[100];
uniform vec3 uAttenuation[100];

uniform int uLightCount;
uniform int uMaxLights;
uniform float uReflectivity;
uniform float uShineDamper;
uniform vec3 uFogColor;
uniform vec3 uSunStrength;

in VertexData {
    vec2 textureCoords;
    vec3 eyeSpacePosition;
    mat3 toTangentSpace;
    float visibility;
	vec4 lightSpaceCoords;
} f_in; 

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outGlow;

float shadowCalculation(vec3 normal) {
    // coordinates in range [-1, 1]
    vec3 shadowMapCoords = f_in.lightSpaceCoords.xyz / f_in.lightSpaceCoords.w;
    // convert to range [0, 1]
    shadowMapCoords = (shadowMapCoords + 1.0) / 2.0;
    float closestDepth = texture(uShadowMap, shadowMapCoords.xy).r;
    float currentDepth = shadowMapCoords.z;

    vec3 lightToPoint = f_in.toTangentSpace * (f_in.eyeSpacePosition - uLightPosition[0]);
    vec3 incidentLight = normalize(lightToPoint);

    float bias = 0.0000; // 0.0001
    //float bias = max(0.0035 * (1.0 - dot(normal, incidentLight)), 0.003);  
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;  
    if(currentDepth > 1.0)
        shadow = 0.0;
    return shadow;
}

void main() 
{
    // Get the blend map colour
    vec4 blackTexture = vec4(0);
    vec4 redTexture = vec4(0);
    vec4 greenTexture = vec4(0);
    vec4 blueTexture = vec4(0);
    
    vec4 blendMapValue = texture(uBlendMap, f_in.textureCoords);
    float blackAmount = 
		1 - (blendMapValue.r + blendMapValue.g + blendMapValue.b);
    vec2 tiledCoords = f_in.textureCoords * 30.0;
    blackTexture = texture(uBaseTexture, tiledCoords) * blackAmount;
    redTexture = texture(uRedTexture, tiledCoords) * blendMapValue.r;
    greenTexture = texture(uGreenTexture, tiledCoords) * blendMapValue.g;
    blueTexture = texture(uBlueTexture, tiledCoords) * blendMapValue.b;

	// normal map for the path texture (blue)
    vec4 normalMapValue = texture(uNormalMap, tiledCoords) * 2.0 - vec4(1.0); 

    vec4 totalColor = blackTexture + redTexture + greenTexture + blueTexture;

	// default normal in tangent space
    vec3 surfaceNormal = vec3(0, 0, 1.0); 
    vec3 normal = normalize(normalMapValue.xyz);
    // mixing the normals (mixing the bump maps)
    vec3 norm = (
		blackAmount * surfaceNormal 
		+ blendMapValue.r * surfaceNormal 
		+ blendMapValue.g * surfaceNormal 
		+ blendMapValue.b * normal
	);
    norm = normalize(norm);

    // convert to tangent space
    vec3 toCamera = f_in.toTangentSpace * (-f_in.eyeSpacePosition);
    toCamera = normalize(toCamera);

    // Light calculation
    float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * uSunStrength;
	ambient = vec3(0.05);

    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    for (int i = 0; i < uLightCount && i < uMaxLights; i++) {
        // convert eyespace to tangent space
        vec3 lightToPoint = 
			f_in.toTangentSpace * (f_in.eyeSpacePosition - uLightPosition[i]);
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
	diffuse = max(diffuse, 0.2);

	float shadow = shadowCalculation(norm);
	vec3 lighting = (1.0 - shadow) * (diffuse + specular);
	//vec3 result = vec3(vec4(ambient + diffuse + specular, 1) * totalColor);
    vec3 result = vec3(vec4(ambient + lighting, 1) * totalColor);
    outColor = vec4(result, 1.0);
	outGlow = vec4(0.0);
}
