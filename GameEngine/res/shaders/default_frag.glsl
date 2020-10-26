#version 330 core

uniform sampler2D textureSampler;

in VertexData {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textureCoords;
} f_in; 

const vec3 lightDirection = vec3(0, -1, -1);
const vec3 lightCol = vec3(1);
const vec3 diffuseCol = vec3(0, 1, 1);
const float ambience = 0.1;
const float specularity = 0.5;
const float shininess = 16;

out vec4 outColor;

void main() {
    // Phong shading
    vec3 ambient = ambience * lightCol;
    vec3 norm = normalize(f_in.normal);
    vec3 lightDir = normalize(lightDirection);
    vec3 diffuse_reflect = lightCol * max(dot(norm, -lightDir), 0.0);

    vec3 reflected = reflect(lightDir, norm);
    vec3 viewDir = normalize(-f_in.position);

    vec3 specular_reflect = specularity * lightCol * pow(max(dot(viewDir, reflected), 0.0), shininess);
    vec3 result = (ambient + diffuse_reflect + specular_reflect) * diffuseCol;
    outColor = vec4(result, 1.0);
}