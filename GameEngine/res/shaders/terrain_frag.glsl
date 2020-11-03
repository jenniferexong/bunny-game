#version 330 core

// Textures
uniform sampler2D uBaseTexture;
uniform sampler2D uRedTexture;
uniform sampler2D uGreenTexture;
uniform sampler2D uBlueTexture;
uniform sampler2D uBlendMap;

uniform vec3 uLightColor;
uniform vec3 uLightPosition;
uniform float uReflectivity;
uniform float uShineDamper;
uniform vec3 uSkyColor;

in VertexData {
    vec3 position;
    vec3 normal;
    vec3 color;
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
    vec2 tiledCoords = f_in.textureCoords * 40.0;
    blackTexture = texture(uBaseTexture, tiledCoords) * blackAmount;
    redTexture = texture(uRedTexture, tiledCoords) * blendMapValue.r;
    greenTexture = texture(uGreenTexture, tiledCoords) * blendMapValue.g;
    blueTexture = texture(uBlueTexture, tiledCoords) * blendMapValue.b;

    vec4 totalColor = blackTexture + redTexture + greenTexture + blueTexture;
    //vec4 totalColor = texture(uBlendMap, tiledCoords);

    // Phong shading
    float ambientStrength = 1;
    vec3 ambient = ambientStrength * uLightColor;

    vec3 norm = normalize(f_in.normal);
    vec3 incidentLight = normalize(f_in.position - uLightPosition);
    
    float diff = max(dot(norm, -incidentLight), 0.0);
    vec3 diffuse = diff * uLightColor;

    vec3 toCamera = normalize(f_in.cameraPosition - f_in.position);
    vec3 reflectDir = reflect(incidentLight, norm);
    float spec = pow(max(dot(toCamera, reflectDir), 0.0), uShineDamper);
    vec3 specular = uReflectivity * spec * uLightColor;

    vec3 result = vec3(vec4(ambient + diffuse + specular, 1) * totalColor);
    //vec3 result = (ambient + diffuse + specular) * f_in.color;
    outColor = mix(vec4(uSkyColor, 1.0), vec4(result, 1.0), f_in.visibility);  // mix with sky colour depending on visibility
}