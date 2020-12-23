#version 330 core

uniform samplerCube uDayTexture;
uniform samplerCube uNightTexture;

uniform float uBlendFactor;

in VertexData {
    vec3 textureCoords;
} f_in;

// For blending the sky into the fog colour
const float lowerLimit = 0.0;
const float upperLimit = 50.0;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outGlow;

void main() 
{
    vec4 day = texture(uDayTexture, f_in.textureCoords);
    vec4 night = texture(uNightTexture, f_in.textureCoords);
    vec4 finalColor = mix(day, night, uBlendFactor);

    // check if the point in the sky is below 
	// the lowerLimit or above the upperLimit
    float factor = (
		(f_in.textureCoords.y - lowerLimit) 
		/ (upperLimit - lowerLimit)
	);

    // 0 means use fog colour, 1 means use texture colour
    factor = clamp(factor, 0.0, 1.0);

    outColor = finalColor;
	outGlow = vec4(0.0);
    //finalColor = 
	//vec4(uSunStrength, 1.0) * mix(vec4(uFogColor, 1.0), finalColor, factor);
}
