#include "TextShader.h"

#include "../Location.h"

const std::string TextShader::vertex_file = "res/shaders/text-vert.glsl";
const std::string TextShader::fragment_file = "res/shaders/text-frag.glsl";

void TextShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::Translation, getUniformLocation("uTranslation") });
	locations_.insert({ UniformVariable::Color, getUniformLocation("uColor") });
	locations_.insert({ UniformVariable::CharacterWidth, getUniformLocation("uCharacterWidth") });
	locations_.insert({ UniformVariable::EdgeTransition, getUniformLocation("uEdgeTransition") });
}

void TextShader::bindAttributes()
{
	bindAttribute(TextAttributeLocation::Position, "aPosition");
	bindAttribute(TextAttributeLocation::Texture, "aTextureCoords");
}

void TextShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}


void TextShader::loadUniforms(const GuiText& text)
{
	// translation
	loadVector(locations_.at(UniformVariable::Translation), text.getPosition());

	// colour
	loadVector(locations_.at(UniformVariable::Color), text.getColor());

	float character_width = (1.f / 50.f) * glm::log(text.getFontSize() + 0.1f) + 0.47f;
	float edge_transition = (1.f / 13.f) * glm::pow(1.45f, -text.getFontSize() + 2.8f) + 0.045f;	

	loadFloat(locations_.at(UniformVariable::CharacterWidth), character_width);
	loadFloat(locations_.at(UniformVariable::EdgeTransition), edge_transition);
}


