
#include "TerrainShader.h"

#include "../renderers/MasterRenderer.h"
#include "../util/Log.h"
#include "../util/Maths.h"
#include "../Location.h"
#include "../environment/Environment.h"

void TerrainShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::glCheck("TerrainShader setUp");
}

void TerrainShader::bindAttributes()
{
	bindAttribute(TerrainAttributeLocation::Position, "aPosition");
	bindAttribute(TerrainAttributeLocation::Normal, "aNormal");
	bindAttribute(TerrainAttributeLocation::Texture, "aTextureCoords");
	bindAttribute(TerrainAttributeLocation::Tangent, "aTangent");

	Error::glCheck("TerrainShader bindAttributes");
}

void TerrainShader::getAllUniformLocations()
{
	INSERT_LOC(TransformationMatrix, "uTransformationMatrix");
	INSERT_LOC(ProjectionMatrix, "uProjectionMatrix");
	INSERT_LOC(ViewMatrix, "uViewMatrix");
	INSERT_LOC(InverseViewMatrix, "uInverseViewMatrix");
	INSERT_LOC(LightColor, "uLightColor");
	INSERT_LOC(LightPosition, "uLightPosition");
	INSERT_LOC(LightCount, "uLightCount");
	INSERT_LOC(MaxLights, "uMaxLights");
	INSERT_LOC(Attenuation, "uAttenuation");
	INSERT_LOC(Reflectivity, "uReflectivity");
	INSERT_LOC(ShineDamper, "uShineDamper");
	INSERT_LOC(FogColor, "uFogColor");
	INSERT_LOC(SunStrength, "uSunStrength");
	INSERT_LOC(BaseTexture, "uBaseTexture");
	INSERT_LOC(RedTexture, "uRedTexture");
	INSERT_LOC(GreenTexture, "uGreenTexture");
	INSERT_LOC(BlueTexture, "uBlueTexture");
	INSERT_LOC(BlendMap, "uBlendMap");
	INSERT_LOC(NormalMap, "uNormalMap");
	INSERT_LOC(ClippingPlane, "uClippingPlane");
	INSERT_LOC(LightSpaceMatrix, "uLightSpaceMatrix");
	INSERT_LOC(ShadowMap, "uShadowMap");
	Error::glCheck("TerrainShader getAllUniformLocations");
}

void TerrainShader::loadUniformPerFrame(
	const Environment& environment,
	glm::vec4 clipping_plane) const
{
	glm::mat4 v_matrix = Maths::createViewMatrix(
		*environment.getCamera().lock());
	// View matrix
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(
		locations_.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));

	// Loading light variables
	int num_lights = environment.getLights().size();
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> attenuations;
	positions.reserve(num_lights);
	colors.reserve(num_lights);
	attenuations.reserve(num_lights);
	for (const auto& l : environment.getLights()) {
		// in eye space
		positions.emplace_back(
			vec3(v_matrix * glm::vec4(l.lock()->getPosition(), 1.f)));  
		colors.emplace_back(l.lock()->getColor());
		attenuations.emplace_back(l.lock()->getAttenuation());
	}

	loadVectors(locations_.at(UniformVariable::LightPosition), positions);
	loadVectors(locations_.at(UniformVariable::LightColor), colors);
	loadVectors(locations_.at(UniformVariable::Attenuation), attenuations);

	loadVector(
		locations_.at(UniformVariable::SunStrength),
		environment.getSun().lock()->getColor()
	);

	loadInt(locations_.at(UniformVariable::LightCount), num_lights);
	loadInt(locations_.at(UniformVariable::MaxLights), Light::max_lights);

	// Loading projection matrix
	loadMatrix(
		locations_.at(UniformVariable::ProjectionMatrix),
		MasterRenderer::projection_matrix
	);

	// load clipping plane
	loadVector(locations_.at(UniformVariable::ClippingPlane), clipping_plane);

	// sky colour
	//loadVector(locations_.at(UniformVariable::FogColor), Application::fog_color);
	Error::glCheck("TerrainShader loadUniformPerFrame");
}

void TerrainShader::loadModelMatrix(const Terrain& terrain) const
{
	// Loading transformation matrix
	glm::vec3 position(terrain.getX(), 0, terrain.getZ());
	glm::mat4 t_matrix = Maths::createTransformationMatrix(
		position, glm::vec3(0), 1, glm::mat4(1)
	);

	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
	Error::glCheck("TerrainShader loadModelMatrix");
}

void TerrainShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(
		locations_.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(
		locations_.at(UniformVariable::ShineDamper), material.shine_damper);

	Error::glCheck("TerrainShader loadMaterial");
}

void TerrainShader::connectTextureUnits() const
{
	loadInt(
		locations_.at(UniformVariable::BaseTexture), TextureLocation::Base);
	loadInt(
		locations_.at(UniformVariable::RedTexture), TextureLocation::Red);
	loadInt(
		locations_.at(UniformVariable::GreenTexture), TextureLocation::Green);
	loadInt(
		locations_.at(UniformVariable::BlueTexture), TextureLocation::Blue);
	loadInt(
		locations_.at(UniformVariable::BlendMap), TextureLocation::BlendMap);
	loadInt(
		locations_.at(UniformVariable::NormalMap), TextureLocation::NormalMap);
	loadInt(
		locations_.at(UniformVariable::ShadowMap), TextureLocation::ShadowMap);

	Error::glCheck("TerrainShader connectTextureUnits");
}
