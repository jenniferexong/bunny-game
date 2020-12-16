#include "HorizontalBlurShader.h"

#include "../../Location.h"
#include "../../Utility.h"

void HorizontalBlurShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void HorizontalBlurShader::getAllUniformLocations()
{
	INSERT_LOC(TargetWidth, "uTargetWidth");
	Error::gl_check("HorizontalBlurShader getAllUniformLocations");
}

void HorizontalBlurShader::loadUniforms(float target_width)
{
	loadFloat(locations_.at(UniformVariable::TargetWidth), target_width);
	Error::gl_check("HorizontalBlurShader loadUniforms");
}

void HorizontalBlurShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::gl_check("HorizontalBlurShader setup");
}
