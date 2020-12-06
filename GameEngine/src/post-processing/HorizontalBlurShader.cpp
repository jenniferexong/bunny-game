#include "HorizontalBlurShader.h"

#include "../Location.h"
#include "../Maths.h"

const std::string HorizontalBlurShader::vertex_file = "res/shaders/post-processing/horizontal-blur-vert.glsl";
const std::string HorizontalBlurShader::fragment_file = "res/shaders/post-processing/blur-frag.glsl";

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