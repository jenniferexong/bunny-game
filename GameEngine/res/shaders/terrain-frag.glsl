#version 330 core

// Textures
uniform sampler2D uBaseTexture;
uniform sampler2D uRedTexture;
uniform sampler2D uGreenTexture;
uniform sampler2D uBlueTexture;
uniform sampler2D uBlendMap;

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
    float visibility;
} f_in; 

out vec4 outColor;

void main() {
    // Get the blend map colour
    vec4 blackTexture = vec4(0);
    vec4 redTexture = vec4(0);
    vec4 greenTexture = vec4(0);
    vec4 blueTexture = vec4(0);
    
    vec4 blendMapValue = texture(uBlendMap, f_in.textureCoords);
    float blackAmount = 1 - (blendMapValue.r + blendMapValue.g + blendMapValue.b);
    vec2 tiledCoords = f_in.textureCoords * 25.0;
    blackTexture = texture(uBaseTexture, tiledCoords) * blackAmount;
    redTexture = texture(uRedTexture, tiledCoords) * blendMapValue.r;
    greenTexture = texture(uGreenTexture, tiledCoords) * blendMapValue.g;
    blueTexture = texture(uBlueTexture, tiledCoords) * blendMapValue.b;

    vec4 totalColor = blackTexture + redTexture + greenTexture + blueTexture;

    vec3 norm = normalize(f_in.normal);
    vec3 toCamera = normalize(f_in.cameraPosition - f_in.position);

    // Light calculation
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * uSunStrength;

    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    for (int i = 0; i < uLightCount && i < uMaxLights; i++) {

        vec3 lightToPoint = f_in.position - uLightPosition[i];
        vec3 incidentLight = normalize(lightToPoint);

        float dist = length(lightToPoint);
        float attenuationFactor = uAttenuation[i].x + (uAttenuation[i].y * dist) + (uAttenuation[i].z * dist * dist);

        float diff = max(dot(norm, -incidentLight), 0.0);
        diffuse += (diff * uLightColor[i]) / attenuationFactor;

        vec3 reflectDir = reflect(incidentLight, norm);
        float spec = pow(max(dot(toCamera, reflectDir), 0.0), uShineDamper);
        specular += (uReflectivity * spec * uLightColor[i]) / attenuationFactor;
    }

    vec3 fogColor = uSunStrength * uFogColor;
    vec3 result = vec3(vec4(ambient + diffuse + specular, 1) * totalColor);
    outColor = mix(vec4(fogColor, 1.0), vec4(result, 1.0), f_in.visibility);  // mix with sky colour depending on visibility
}