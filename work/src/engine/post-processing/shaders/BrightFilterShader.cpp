#include "BrightFilterShader.h"

#include "../../Location.h"
#include "../../Utility.h"

void BrightFilterShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void BrightFilterShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::gl_check("BrightFilterShader setup");
}
