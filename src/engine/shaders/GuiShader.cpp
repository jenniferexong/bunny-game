
#include "GuiShader.h"

#include "../Location.h"
#include "../util/Log.h"
#include "../util/Maths.h"
#include "../ui/GuiTexture.h"

void GuiShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void GuiShader::getAllUniformLocations()
{
	INSERT_LOC(TransformationMatrix, "uTransformationMatrix");
	INSERT_LOC(FlipVertically, "uFlipVertically");
	INSERT_LOC(HasTexture, "uHasTexture");
	INSERT_LOC(Color, "uColor");
	Error::glCheck("GuiShader getAllUniformLocations");
}

void GuiShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);

	Error::glCheck("GuiShader setup");
}

void GuiShader::loadUniforms(const GuiTexture& gui) const
{
	glm::mat4 t_matrix = Maths::createTransformationMatrix(
		gui.getPosition(), gui.getRotation(), gui.getScale());

	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
	loadBoolean(
		locations_.at(UniformVariable::FlipVertically), gui.flipVertically());
	loadBoolean(
		locations_.at(UniformVariable::HasTexture), gui.hasTexture());
	loadVector(locations_.at(UniformVariable::Color), gui.getColor());

	Error::glCheck("GuiShader loadUniforms");
}
