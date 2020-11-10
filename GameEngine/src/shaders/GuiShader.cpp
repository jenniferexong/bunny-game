
#include "GuiShader.h"

#include "../Location.h"
#include "../Maths.h"

const std::string GuiShader::vertex_file = "res/shaders/gui-vert.glsl";
const std::string GuiShader::fragment_file = "res/shaders/gui-frag.glsl";

void GuiShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void GuiShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
}

void GuiShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

void GuiShader::loadModelMatrix(const GuiTexture& gui) const
{
	glm::mat4 t_matrix = Maths::createTransformationMatrix(gui.getPosition(), gui.getRotation(), gui.getScale());
	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
}