#include "VerticalBlurShader.h"

#include "../Location.h"
#include "../Maths.h"

const std::string VerticalBlurShader::vertex_file = "res/shaders/post-processing/vertical-blur-vert.glsl";
const std::string VerticalBlurShader::fragment_file = "res/shaders/post-processing/blur-frag.glsl";

void VerticalBlurShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void VerticalBlurShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TargetHeight, getUniformLocation("uTargetHeight") });
}

void VerticalBlurShader::loadUniforms(float target_height)
{
	loadFloat(locations_.at(UniformVariable::TargetHeight), target_height);
}

void VerticalBlurShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}