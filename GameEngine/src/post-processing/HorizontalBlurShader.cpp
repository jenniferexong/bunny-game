#include "HorizontalBlurShader.h"

#include "../Location.h"
#include "../Maths.h"

void HorizontalBlurShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void HorizontalBlurShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TargetWidth, getUniformLocation("uTargetWidth") });
}

void HorizontalBlurShader::loadUniforms(float target_width)
{
	loadFloat(locations_.at(UniformVariable::TargetWidth), target_width);
}

void HorizontalBlurShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}