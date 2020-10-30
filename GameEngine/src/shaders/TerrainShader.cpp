
#include "TerrainShader.h"

const std::string TerrainShader::s_vertex_file = "res/shaders/terrain_vert.glsl";
const std::string TerrainShader::s_fragment_file = "res/shaders/terrain_frag.glsl";

void TerrainShader::setUp()
{
	Shader::setUp(s_vertex_file, s_fragment_file);
}

void TerrainShader::bindAttributes()
{
	bindAttribute(ePosition, "position");
	bindAttribute(eNormal, "normal");
	bindAttribute(eTexture, "textureCoords");
}

void TerrainShader::getAllUniformLocations()
{

}