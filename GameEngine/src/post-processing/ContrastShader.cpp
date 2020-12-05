#include "ContrastShader.h"

#include "../Location.h"
#include "../Maths.h"

const std::string ContrastShader::vertex_file = "res/shaders/post-processing/contrast-vert.glsl";
const std::string ContrastShader::fragment_file = "res/shaders/post-processing/contrast-frag.glsl";

void ContrastShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void ContrastShader::getAllUniformLocations()
{
}

void ContrastShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}
