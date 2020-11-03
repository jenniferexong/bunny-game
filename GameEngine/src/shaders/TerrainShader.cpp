
#include "TerrainShader.h"

#include "../Maths.h"
#include "../Application.h"
#include "../Location.h"

const std::string TerrainShader::s_vertex_file = "res/shaders/terrain_vert.glsl";
const std::string TerrainShader::s_fragment_file = "res/shaders/terrain_frag.glsl";

void TerrainShader::setUp()
{
	Shader::setUp(s_vertex_file, s_fragment_file);
}

void TerrainShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "position");
	bindAttribute(AttributeLocation::Normal, "normal");
	bindAttribute(AttributeLocation::Texture, "textureCoords");
}

void TerrainShader::getAllUniformLocations()
{
	printf("loading locations into map\n");
	m_locations.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	m_locations.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	m_locations.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	m_locations.insert({ UniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	m_locations.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	m_locations.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	m_locations.insert({ UniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	m_locations.insert({ UniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	m_locations.insert({ UniformVariable::SkyColor, getUniformLocation("uSkyColor") });
	m_locations.insert({ UniformVariable::BaseTexture, getUniformLocation("uBaseTexture") });
	m_locations.insert({ UniformVariable::RedTexture, getUniformLocation("uRedTexture") });
	m_locations.insert({ UniformVariable::GreenTexture, getUniformLocation("uGreenTexture") });
	m_locations.insert({ UniformVariable::BlueTexture, getUniformLocation("uBlueTexture") });
	m_locations.insert({ UniformVariable::BlendMap, getUniformLocation("uBlendMap") });
	printf("size: %d\n", m_locations.size());
}

void TerrainShader::loadUniformPerFrame(const Light& light) const
{
	// Loading light variables
	loadVector(m_locations.at(UniformVariable::LightPosition), light.getPosition());
	loadVector(m_locations.at(UniformVariable::LightColor), light.getColor());

	// Loading projection matrix
	loadMatrix(m_locations.at(UniformVariable::ProjectionMatrix), MasterRenderer::s_projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::s_camera);
	loadMatrix(m_locations.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(m_locations.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));

	// sky colour
	loadVector(m_locations.at(UniformVariable::SkyColor), Application::s_sky_color);
}

void TerrainShader::loadModelMatrix(const Terrain& terrain) const
{
	// Loading transformation matrix
	glm::vec3 position(terrain.getX(), 0, terrain.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(position, glm::vec3(0), 1);

	loadMatrix(m_locations.at(UniformVariable::TransformationMatrix), t_matrix);
}

void TerrainShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(m_locations.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(m_locations.at(UniformVariable::ShineDamper), material.shine_damper);
}

void TerrainShader::connectTextureUnits() const
{
	loadInt(m_locations.at(UniformVariable::BaseTexture), 0);
	loadInt(m_locations.at(UniformVariable::RedTexture), 1);
	loadInt(m_locations.at(UniformVariable::GreenTexture), 2);
	loadInt(m_locations.at(UniformVariable::BlueTexture), 3);
	loadInt(m_locations.at(UniformVariable::BlendMap), 4);
}
