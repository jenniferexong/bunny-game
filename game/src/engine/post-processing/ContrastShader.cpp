#include "ContrastShader.h"

#include "../Location.h"
#include "../Utility.h"

void ContrastShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void ContrastShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);

	Error::gl_check("ContrastShader setup");
}
