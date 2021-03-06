#version 330 core

uniform sampler2D textureAtlas;

uniform vec3 uColor;
uniform float uCharacterWidth;
uniform float uEdgeTransition;

// Outline and glow effect
uniform vec3 uBorderColor;
uniform float uBorderWidth;
uniform float uBorderEdgeTransition;

in VertexData {
    vec2 textureCoords;
} f_in;

out vec4 outColor;

void main() 
{
	// get distance information
    float distance = 1.0 - texture(textureAtlas, f_in.textureCoords).a; 
    float alpha = (
		1.0 - 
		smoothstep(uCharacterWidth, uCharacterWidth + uEdgeTransition, distance)
	);
    float outlineAlpha = (
		1.0 - 
		smoothstep(uBorderWidth, uBorderWidth + uBorderEdgeTransition, distance)
	);

	// add glow effect
    if (uEdgeTransition < uBorderEdgeTransition) { 
        float amount = (distance - uBorderWidth) / uBorderEdgeTransition;
        outlineAlpha = pow(10000, -amount);
        outlineAlpha = max(outlineAlpha - 0.01, 0);
    }

    float finalAlpha = alpha + (1.0 - alpha) * outlineAlpha;
    vec3 finalColor = mix(uBorderColor, uColor, alpha / finalAlpha);

    outColor = vec4(finalColor, finalAlpha);
}
