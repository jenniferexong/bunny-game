
#include "EntityShader.h"

#include "../renderers/MasterRenderer.h"
#include "../Location.h"
#include "../Utility.h"
#include "../Environment.h"

void EntityShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::gl_check(name);
}

void EntityShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
	bindAttribute(AttributeLocation::Normal, "aNormal");
	bindAttribute(AttributeLocation::Texture, "aTextureCoords");
	bindAttribute(AttributeLocation::ModelMatrixColumn1, "aModelMatrices");
	bindAttribute(AttributeLocation::ModelBrightness, "aModelBrightness");
}

void EntityShader::getAllUniformLocations()
{
	INSERT_LOC(TransformationMatrix, "uTransformationMatrix");
	INSERT_LOC(ProjectionMatrix, "uProjectionMatrix");
	INSERT_LOC(ViewMatrix, "uViewMatrix");
	INSERT_LOC(ViewMatrix, "uViewMatrix");
	INSERT_LOC(InverseViewMatrix, "uInverseViewMatrix");
	INSERT_LOC(LightColor, "uLightColor");
	INSERT_LOC(LightPosition, "uLightPosition");
	INSERT_LOC(LightCount, "uLightCount");
	INSERT_LOC(MaxLights, "uMaxLights");
	INSERT_LOC(Attenuation, "uAttenuation");
	INSERT_LOC(Reflectivity, "uReflectivity");
	INSERT_LOC(ShineDamper, "uShineDamper");
	INSERT_LOC(FakeLighting, "uFakeLighting");
	INSERT_LOC(FogColor, "uFogColor");
	INSERT_LOC(SunStrength, "uSunStrength");
	INSERT_LOC(ClippingPlane, "uClippingPlane");
}

void EntityShader::loadUniformPerFrame(
	const Environment& environment,
	glm::vec4 clipping_plane) const
{
	// Loading light variables
	int num_lights = environment.getLights().size();
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> attenuations;
	positions.reserve(num_lights);
	colors.reserve(num_lights);
	attenuations.reserve(num_lights);
	for (const auto& l: environment.getLights()) {
		positions.emplace_back(l.lock()->getPosition());
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

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(
		*environment.getCamera().lock()
	);
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(locations_.at(
		UniformVariable::InverseViewMatrix),
		inverse(v_matrix)
	);

	// load clipping plane
	loadVector(locations_.at(UniformVariable::ClippingPlane), clipping_plane);
	 
	// Sky colour
	//loadVector(locations_.at(UniformVariable::FogColor), Application::fog_color);
}

void EntityShader::loadModelMatrix(const Entity& entity) const
{
	// Loading transformation matrix
	glm::mat4 t_matrix = Maths::createTransformationMatrix(
		entity.getPosition(), 
		entity.getActualRotation(),
		entity.getScale(),
		entity.getAlignmentRotation()
	);
	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
}

void EntityShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(
		locations_.at(UniformVariable::Reflectivity), material.reflectivity
	);
	loadFloat(
		locations_.at(UniformVariable::ShineDamper), material.shine_damper
	);
	loadBoolean(
		locations_.at(UniformVariable::FakeLighting), material.uses_fake_lighting
	);
}

