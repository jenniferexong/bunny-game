#include "ContrastShader.h"

#include "../../Location.h"
#include "../../util/Log.h"

void ContrastShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void ContrastShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::glCheck("ContrastShader setup");
}
