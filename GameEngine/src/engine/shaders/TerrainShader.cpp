
#include "TerrainShader.h"

#include "../renderers/MasterRenderer.h"
#include "../Utility.h"
#include "../Location.h"
#include "../Environment.h"

void TerrainShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

void TerrainShader::bindAttributes()
{
	bindAttribute(TerrainAttributeLocation::Position, "aPosition");
	bindAttribute(TerrainAttributeLocation::Normal, "aNormal");
	bindAttribute(TerrainAttributeLocation::Texture, "aTextureCoords");
	bindAttribute(TerrainAttributeLocation::Tangent, "aTangent");
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
	locations_.insert({ UniformVariable::FogColor, getUniformLocation("uFogColor") });
	locations_.insert({ UniformVariable::SunStrength, getUniformLocation("uSunStrength") });

	locations_.insert({ UniformVariable::BaseTexture, getUniformLocation("uBaseTexture") });
	locations_.insert({ UniformVariable::RedTexture, getUniformLocation("uRedTexture") });
	locations_.insert({ UniformVariable::GreenTexture, getUniformLocation("uGreenTexture") });
	locations_.insert({ UniformVariable::BlueTexture, getUniformLocation("uBlueTexture") });
	locations_.insert({ UniformVariable::BlendMap, getUniformLocation("uBlendMap") });
	locations_.insert({ UniformVariable::NormalMap, getUniformLocation("uNormalMap") });

	locations_.insert({ UniformVariable::ClippingPlane, getUniformLocation("uClippingPlane") });
}

void TerrainShader::loadUniformPerFrame(const Environment& environment, glm::vec4 clipping_plane) const
{
	glm::mat4 v_matrix = Maths::createViewMatrix(*environment.getCamera().lock());
	// View matrix
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(locations_.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));

	// Loading light variables
	int num_lights = environment.getLights().size();
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> attenuations;
	positions.reserve(num_lights);
	colors.reserve(num_lights);
	attenuations.reserve(num_lights);
	for (const auto& l : environment.getLights()) {
		positions.emplace_back(vec3(v_matrix * glm::vec4(l.lock()->getPosition(), 1.f)));  // in eye space
		colors.emplace_back(l.lock()->getColor());
		attenuations.emplace_back(l.lock()->getAttenuation());
	}

	loadVectors(locations_.at(UniformVariable::LightPosition), positions);
	loadVectors(locations_.at(UniformVariable::LightColor), colors);
	loadVectors(locations_.at(UniformVariable::Attenuation), attenuations);

	loadVector(locations_.at(UniformVariable::SunStrength), environment.getSun().lock()->getColor());

	loadInt(locations_.at(UniformVariable::LightCount), num_lights);
	loadInt(locations_.at(UniformVariable::MaxLights), Light::max_lights);

	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// load clipping plane
	loadVector(locations_.at(UniformVariable::ClippingPlane), clipping_plane);

	// sky colour
	//loadVector(locations_.at(UniformVariable::FogColor), Application::fog_color);
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
	loadInt(locations_.at(UniformVariable::NormalMap), TextureLocation::NormalMap);
}
