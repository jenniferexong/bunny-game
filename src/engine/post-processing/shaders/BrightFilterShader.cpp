#include "BrightFilterShader.h"

#include "../../Location.h"
#include "../../util/Log.h"

void BrightFilterShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void BrightFilterShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::glCheck("BrightFilterShader setup");
}
