
#include "TerrainShader.h"

#include "../Maths.h"
#include "../Application.h"
#include "../Location.h"

const std::string TerrainShader::vertex_file = "res/shaders/terrain_vert.glsl";
const std::string TerrainShader::fragment_file = "res/shaders/terrain_frag.glsl";

void TerrainShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
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
	_locations.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	_locations.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	_locations.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	_locations.insert({ UniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	_locations.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	_locations.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	_locations.insert({ UniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	_locations.insert({ UniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	_locations.insert({ UniformVariable::SkyColor, getUniformLocation("uSkyColor") });
	_locations.insert({ UniformVariable::BaseTexture, getUniformLocation("uBaseTexture") });
	_locations.insert({ UniformVariable::RedTexture, getUniformLocation("uRedTexture") });
	_locations.insert({ UniformVariable::GreenTexture, getUniformLocation("uGreenTexture") });
	_locations.insert({ UniformVariable::BlueTexture, getUniformLocation("uBlueTexture") });
	_locations.insert({ UniformVariable::BlendMap, getUniformLocation("uBlendMap") });
	printf("size: %d\n", _locations.size());
}

void TerrainShader::loadUniformPerFrame(const Light& light) const
{
	// Loading light variables
	loadVector(_locations.at(UniformVariable::LightPosition), light.getPosition());
	loadVector(_locations.at(UniformVariable::LightColor), light.getColor());

	// Loading projection matrix
	loadMatrix(_locations.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::camera);
	loadMatrix(_locations.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(_locations.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));

	// sky colour
	loadVector(_locations.at(UniformVariable::SkyColor), Application::sky_color);
}

void TerrainShader::loadModelMatrix(const Terrain& terrain) const
{
	// Loading transformation matrix
	glm::vec3 position(terrain.getX(), 0, terrain.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(position, glm::vec3(0), 1);

	loadMatrix(_locations.at(UniformVariable::TransformationMatrix), t_matrix);
}

void TerrainShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(_locations.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(_locations.at(UniformVariable::ShineDamper), material.shine_damper);
}

void TerrainShader::connectTextureUnits() const
{
	loadInt(_locations.at(UniformVariable::BaseTexture), TextureLocation::Base);
	loadInt(_locations.at(UniformVariable::RedTexture), TextureLocation::Red);
	loadInt(_locations.at(UniformVariable::GreenTexture), TextureLocation::Green);
	loadInt(_locations.at(UniformVariable::BlueTexture), TextureLocation::Blue);
	loadInt(_locations.at(UniformVariable::BlendMap), TextureLocation::BlendMap);
}
