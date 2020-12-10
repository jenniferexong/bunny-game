#include "ContrastShader.h"

#include "../Location.h"

void ContrastShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void ContrastShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}
