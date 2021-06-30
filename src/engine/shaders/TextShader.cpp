#include "TextShader.h"

#include "../Location.h"
#include "../ui/GuiText.h"
#include "../util/Log.h"
#include "../util/Maths.h"

void TextShader::getAllUniformLocations()
{
	INSERT_LOC(Translation, "uTranslation");
	INSERT_LOC(Color, "uColor");
	INSERT_LOC(BorderColor, "uBorderColor");
	INSERT_LOC(CharacterWidth, "uCharacterWidth");
	INSERT_LOC(EdgeTransition, "uEdgeTransition");
	INSERT_LOC(BorderWidth, "uBorderWidth");
	INSERT_LOC(BorderEdgeTransition, "uBorderEdgeTransition");
	Error::glCheck("TextShader getAllUniformLocations");
}

void TextShader::bindAttributes()
{
	bindAttribute(TextAttributeLocation::Position, "aPosition");
	bindAttribute(TextAttributeLocation::Texture, "aTextureCoords");
}

void TextShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::glCheck(name);
}

void TextShader::loadUniforms(const GuiText& text)
{
	// translation
	loadVector(
		locations_.at(UniformVariable::Translation), text.getPosition()
	);

	// colour
	loadVector(locations_.at(UniformVariable::Color), text.getColor());
	loadVector(
		locations_.at(UniformVariable::BorderColor), text.getBorderColor()
	);

	loadFloat(
		locations_.at(UniformVariable::CharacterWidth), text.getCharacterWidth()
	);
	loadFloat(
		locations_.at(UniformVariable::EdgeTransition), text.getEdgeTransition()
	);
	loadFloat(
		locations_.at(UniformVariable::BorderWidth), text.getBorderWidth()
	);
	loadFloat(
		locations_.at(UniformVariable::BorderEdgeTransition),
		text.getBorderEdgeTransition()
	);

	Error::glCheck("TextShader loadUniforms");
}


