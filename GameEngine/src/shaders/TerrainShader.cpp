
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
	locations_.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	locations_.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	locations_.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	locations_.insert({ UniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	locations_.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	locations_.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	locations_.insert({ UniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	locations_.insert({ UniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	locations_.insert({ UniformVariable::SkyColor, getUniformLocation("uSkyColor") });
	locations_.insert({ UniformVariable::BaseTexture, getUniformLocation("uBaseTexture") });
	locations_.insert({ UniformVariable::RedTexture, getUniformLocation("uRedTexture") });
	locations_.insert({ UniformVariable::GreenTexture, getUniformLocation("uGreenTexture") });
	locations_.insert({ UniformVariable::BlueTexture, getUniformLocation("uBlueTexture") });
	locations_.insert({ UniformVariable::BlendMap, getUniformLocation("uBlendMap") });
}

void TerrainShader::loadUniformPerFrame(const Light& light) const
{
	// Loading light variables
	loadVector(locations_.at(UniformVariable::LightPosition), light.getPosition());
	loadVector(locations_.at(UniformVariable::LightColor), light.getColor());

	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::camera);
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(locations_.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));

	// sky colour
	loadVector(locations_.at(UniformVariable::SkyColor), Application::sky_color);
}

void TerrainShader::loadModelMatrix(const Terrain& terrain) const
{
	// Loading transformation matrix
	glm::vec3 position(terrain.getX(), 0, terrain.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(position, glm::vec3(0), 1);

	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
}

void TerrainShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(locations_.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(locations_.at(UniformVariable::ShineDamper), material.shine_damper);
}

void TerrainShader::connectTextureUnits() const
{
	loadInt(locations_.at(UniformVariable::BaseTexture), TextureLocation::Base);
	loadInt(locations_.at(UniformVariable::RedTexture), TextureLocation::Red);
	loadInt(locations_.at(UniformVariable::GreenTexture), TextureLocation::Green);
	loadInt(locations_.at(UniformVariable::BlueTexture), TextureLocation::Blue);
	loadInt(locations_.at(UniformVariable::BlendMap), TextureLocation::BlendMap);
}
