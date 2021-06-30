#include "CombineShader.h"

#include "../../Location.h"
#include "../../util/Log.h"

void CombineShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void CombineShader::connectTextureUnits()
{
	loadInt(locations_.at(UniformVariable::ColorTexture), 0);
	loadInt(locations_.at(UniformVariable::HighlightTexture), 1);
	Error::glCheck("CombineShader connectTextureUnits");
}

void CombineShader::getAllUniformLocations() 
{
	INSERT_LOC(ColorTexture, "uColorTexture");
	INSERT_LOC(HighlightTexture, "uHighlightTexture");
	Error::glCheck("CombineShader getAllUniformLocations");
}

void CombineShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	start();
	connectTextureUnits();
	stop();
	Error::glCheck("CombineShader setup");
}
