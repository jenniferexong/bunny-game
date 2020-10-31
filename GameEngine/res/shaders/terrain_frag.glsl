#version 330 core

// Uniform data
uniform sampler2D textureSampler;

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
    // Phong shading
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * uLightColor;

    vec3 norm = normalize(f_in.normal);
    vec3 incidentLight = normalize(f_in.position - uLightPosition);
    
    float diff = max(dot(norm, -incidentLight), 0.0);
    vec3 diffuse = diff * uLightColor;

    vec3 toCamera = normalize(f_in.cameraPosition - f_in.position);
    vec3 reflectDir = reflect(incidentLight, norm);
    float spec = pow(max(dot(toCamera, reflectDir), 0.0), uShineDamper);
    vec3 specular = uReflectivity * spec * uLightColor;

    vec3 result = vec3(vec4(ambient + diffuse + specular, 1) * texture(textureSampler, f_in.textureCoords));
    //vec3 result = (ambient + diffuse + specular) * f_in.color;
    outColor = mix(vec4(uSkyColor, 1.0), vec4(result, 1.0), f_in.visibility);  // mix with sky colour depending on visibility
}