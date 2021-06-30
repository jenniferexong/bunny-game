#include "VerticalBlurShader.h"

#include "../../Location.h"
#include "../../util/Log.h"

void VerticalBlurShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void VerticalBlurShader::getAllUniformLocations()
{
	INSERT_LOC(TargetHeight, "uTargetHeight");
	Error::glCheck("VerticalBlurShader getAllUniformLocations");
}

void VerticalBlurShader::loadUniforms(float target_height)
{
	loadFloat(locations_.at(UniformVariable::TargetHeight), target_height);
	Error::glCheck("VerticalBlurShader loadUniforms");
}

void VerticalBlurShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::glCheck("VerticalBlurShader setup");
}
