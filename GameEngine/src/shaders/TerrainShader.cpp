
#include "TerrainShader.h"

#include "../Maths.h"
#include "../Application.h"
#include "../AttributeLocation.h"

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
	printf("loading locations into map\n");
	m_locations.insert({ EUniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	m_locations.insert({ EUniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	m_locations.insert({ EUniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	m_locations.insert({ EUniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	m_locations.insert({ EUniformVariable::LightColor, getUniformLocation("uLightColor") });
	m_locations.insert({ EUniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	m_locations.insert({ EUniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	m_locations.insert({ EUniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	m_locations.insert({ EUniformVariable::SkyColor, getUniformLocation("uSkyColor") });
	m_locations.insert({ EUniformVariable::BaseTexture, getUniformLocation("uBaseTexture") });
	m_locations.insert({ EUniformVariable::RedTexture, getUniformLocation("uRedTexture") });
	m_locations.insert({ EUniformVariable::GreenTexture, getUniformLocation("uGreenTexture") });
	m_locations.insert({ EUniformVariable::BlueTexture, getUniformLocation("uBlueTexture") });
	m_locations.insert({ EUniformVariable::BlendMap, getUniformLocation("uBlendMap") });
	printf("size: %d\n", m_locations.size());
}

void TerrainShader::loadUniformPerFrame(const Light& light) const
{
	// Loading light variables
	loadVector(m_locations.at(EUniformVariable::LightPosition), light.getPosition());
	loadVector(m_locations.at(EUniformVariable::LightColor), light.getColor());

	// Loading projection matrix
	loadMatrix(m_locations.at(EUniformVariable::ProjectionMatrix), MasterRenderer::s_projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::s_camera);
	loadMatrix(m_locations.at(EUniformVariable::ViewMatrix), v_matrix);
	loadMatrix(m_locations.at(EUniformVariable::InverseViewMatrix), inverse(v_matrix));

	// sky colour
	loadVector(m_locations.at(EUniformVariable::SkyColor), Application::s_sky_color);
}

void TerrainShader::loadModelMatrix(const Terrain& terrain) const
{
	// Loading transformation matrix
	glm::vec3 position(terrain.getX(), 0, terrain.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(position, glm::vec3(0), 1);

	loadMatrix(m_locations.at(EUniformVariable::TransformationMatrix), t_matrix);
}

void TerrainShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(m_locations.at(EUniformVariable::Reflectivity), material.reflectivity);
	loadFloat(m_locations.at(EUniformVariable::ShineDamper), material.shine_damper);
}

void TerrainShader::connectTextureUnits() const
{
	loadInt(m_locations.at(EUniformVariable::BaseTexture), eBase);
	loadInt(m_locations.at(EUniformVariable::RedTexture), eRed);
	loadInt(m_locations.at(EUniformVariable::GreenTexture), eGreen);
	loadInt(m_locations.at(EUniformVariable::BlueTexture), eBlue);
	loadInt(m_locations.at(EUniformVariable::BlendMap), eBlendMap);
}

/*
void TerrainShader::loadLight(const Light& light) const
{
	// Loading light variables
	loadVector(m_locations.at(EUniformVariable::LightPosition), light.getPosition());
	loadVector(m_locations.at(EUniformVariable::LightColor), light.getColor());
}

void TerrainShader::loadViewProjection() const
{
	// Loading projection matrix
	loadMatrix(m_locations.at(EUniformVariable::ProjectionMatrix), MasterRenderer::s_projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::s_camera);
	loadMatrix(m_locations.at(EUniformVariable::ViewMatrix), v_matrix);
	loadMatrix(m_locations.at(EUniformVariable::InverseViewMatrix), inverse(v_matrix));
}
*/

