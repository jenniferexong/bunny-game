#include "HorizontalBlurShader.h"

#include "../../Location.h"
#include "../../util/Log.h"

void HorizontalBlurShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void HorizontalBlurShader::getAllUniformLocations()
{
	INSERT_LOC(TargetWidth, "uTargetWidth");
	Error::glCheck("HorizontalBlurShader getAllUniformLocations");
}

void HorizontalBlurShader::loadUniforms(float target_width)
{
	loadFloat(locations_.at(UniformVariable::TargetWidth), target_width);
	Error::glCheck("HorizontalBlurShader loadUniforms");
}

void HorizontalBlurShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::glCheck("HorizontalBlurShader setup");
}
