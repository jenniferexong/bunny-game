#version 330 core

// Uniform data
uniform sampler2D textureSampler;

uniform vec3 uLightColor[100];
uniform vec3 uLightPosition[100];
uniform int uLightCount;
uniform int uMaxLights;

uniform float uReflectivity;
uniform float uShineDamper;
uniform vec3 uSkyColor;

in VertexData {
    vec3 position;
    vec3 normal;
    vec2 textureCoords;
    vec3 cameraPosition;
    float visibility;
} f_in; 

out vec4 outColor;

void main() {
    // Phong shading
    vec4 textureCol = texture(textureSampler, f_in.textureCoords);
    if (textureCol.a < 0.5) {
        discard;
    }
    vec3 toCamera = normalize(f_in.cameraPosition - f_in.position);
    vec3 norm = normalize(f_in.normal);

    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * vec3(1);

    // Lighting calculations
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    for (int i = 0; i < uLightCount && i < uMaxLights; i++) {
        vec3 incidentLight = normalize(f_in.position - uLightPosition[i]);
        
        float diff = max(dot(norm, -incidentLight), 0.0);
        diffuse += diff * uLightColor[i];

        vec3 reflectDir = reflect(incidentLight, norm);
        float spec = pow(max(dot(toCamera, reflectDir), 0.0), uShineDamper);
        specular += uReflectivity * spec * uLightColor[i];
    }

    vec3 result = vec3(vec4(ambient + diffuse + specular, 1) * texture(textureSampler, f_in.textureCoords));
    outColor = mix(vec4(uSkyColor, 1.0), vec4(result, 1.0), f_in.visibility);  // mix with sky colour depending on visibility
}