#include "ShadowShader.h"

#include "../Location.h"

void ShadowShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::Matrix, getUniformLocation("uMatrix") });
}

void ShadowShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void ShadowShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

void ShadowShader::loadUniforms(glm::mat4 matrix)
{
	// translation
	loadMatrix(locations_.at(UniformVariable::Matrix), matrix);
}
