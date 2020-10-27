#version 330 core

// Uniform data
uniform sampler2D textureSampler;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;

in VertexData {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textureCoords;
} f_in; 

out vec4 outColor;

void main() {
    // Phong shading
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;

    vec3 normal = normalize(f_in.normal);
    vec3 incidentLight = normalize(f_in.position - uLightPosition);
    
    float cosA = max(dot(normal, -incidentLight), 0.0);
    vec3 diffuse = cosA * uLightColor;

    vec3 result = (ambient + diffuse) * f_in.color;
    outColor = vec4(result, 1.0); 
}