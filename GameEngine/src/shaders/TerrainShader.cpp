
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
	bindAttribute(AttributeLocation::Position, "aPosition");
	bindAttribute(AttributeLocation::Normal, "aNormal");
	bindAttribute(AttributeLocation::Texture, "aTextureCoords");
}

void TerrainShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	locations_.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	locations_.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	locations_.insert({ UniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });

	locations_.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	locations_.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	locations_.insert({ UniformVariable::LightCount, getUniformLocation("uLightCount") });
	locations_.insert({ UniformVariable::MaxLights, getUniformLocation("uMaxLights") });
	locations_.insert({ UniformVariable::Attenuation, getUniformLocation("uAttenuation") });

	locations_.insert({ UniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	locations_.insert({ UniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	locations_.insert({ UniformVariable::SkyColor, getUniformLocation("uSkyColor") });

	locations_.insert({ UniformVariable::BaseTexture, getUniformLocation("uBaseTexture") });
	locations_.insert({ UniformVariable::RedTexture, getUniformLocation("uRedTexture") });
	locations_.insert({ UniformVariable::GreenTexture, getUniformLocation("uGreenTexture") });
	locations_.insert({ UniformVariable::BlueTexture, getUniformLocation("uBlueTexture") });
	locations_.insert({ UniformVariable::BlendMap, getUniformLocation("uBlendMap") });
}

void TerrainShader::loadUniformPerFrame(const std::vector<Light>& lights) const
{
	// Loading light variables
	int num_lights = lights.size();
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> attenuations;
	positions.reserve(num_lights);
	colors.reserve(num_lights);
	attenuations.reserve(num_lights);
	for (const auto& l : lights) {
		positions.emplace_back(l.getPosition());
		colors.emplace_back(l.getColor());
		attenuations.emplace_back(l.getAttenuation());
	}

	loadVectors(locations_.at(UniformVariable::LightPosition), positions);
	loadVectors(locations_.at(UniformVariable::LightColor), colors);
	loadVectors(locations_.at(UniformVariable::Attenuation), attenuations);
	loadInt(locations_.at(UniformVariable::LightCount), num_lights);
	loadInt(locations_.at(UniformVariable::MaxLights), Light::max_lights);

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
	glm::mat4 t_matrix = Maths::createTransformationMatrix(position, glm::vec3(0), 1, glm::mat4(1));

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
