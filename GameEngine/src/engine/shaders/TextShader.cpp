#include "TextShader.h"

#include "../../ui/GuiText.h"
#include "../Location.h"

void TextShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::Translation, getUniformLocation("uTranslation") });
	locations_.insert({ UniformVariable::Color, getUniformLocation("uColor") });
	locations_.insert({ UniformVariable::BorderColor, getUniformLocation("uBorderColor") });
	locations_.insert({ UniformVariable::CharacterWidth, getUniformLocation("uCharacterWidth") });
	locations_.insert({ UniformVariable::EdgeTransition, getUniformLocation("uEdgeTransition") });
	locations_.insert({ UniformVariable::BorderWidth, getUniformLocation("uBorderWidth") });
	locations_.insert({ UniformVariable::BorderEdgeTransition, getUniformLocation("uBorderEdgeTransition") });
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
	loadVector(locations_.at(UniformVariable::BorderColor), text.getBorderColor());

	loadFloat(locations_.at(UniformVariable::CharacterWidth), text.getCharacterWidth());
	loadFloat(locations_.at(UniformVariable::EdgeTransition), text.getEdgeTransition());
	loadFloat(locations_.at(UniformVariable::BorderWidth), text.getBorderWidth());
	loadFloat(locations_.at(UniformVariable::BorderEdgeTransition), text.getBorderEdgeTransition());
}


