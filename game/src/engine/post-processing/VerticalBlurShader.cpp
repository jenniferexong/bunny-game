#include "VerticalBlurShader.h"

#include "../Location.h"
#include "../Utility.h"

void VerticalBlurShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void VerticalBlurShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TargetHeight, getUniformLocation("uTargetHeight") });
	Error::gl_check("VerticalBlurShader getAllUniformLocations");
}

void VerticalBlurShader::loadUniforms(float target_height)
{
	loadFloat(locations_.at(UniformVariable::TargetHeight), target_height);
	Error::gl_check("VerticalBlurShader loadUniforms");
}

void VerticalBlurShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::gl_check("VerticalBlurShader setup");
}
