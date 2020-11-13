
#include "EntityShader.h"

#include "../render_engine/MasterRenderer.h"
#include "../Application.h"
#include "../Location.h"

const std::string EntityShader::vertex_file = "res/shaders/entity-vert.glsl";
const std::string EntityShader::fragment_file = "res/shaders/entity-frag.glsl";

void EntityShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
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
	locations_.insert({ UniformVariable::FakeLighting, getUniformLocation("uFakeLighting") });
	locations_.insert({ UniformVariable::FogColor, getUniformLocation("uFogColor") });
	locations_.insert({ UniformVariable::SunStrength, getUniformLocation("uSunStrength") });
}

void EntityShader::loadUniformPerFrame(const shared_ptr<Scene>& scene) const
{
	// Loading light variables
	int num_lights = scene->getLights().size();
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> attenuations;
	positions.reserve(num_lights);
	colors.reserve(num_lights);
	attenuations.reserve(num_lights);
	for (const auto& l: scene->getLights()) {
		positions.emplace_back(l.getPosition());
		colors.emplace_back(l.getColor());
		attenuations.emplace_back(l.getAttenuation());
	}

	loadVectors(locations_.at(UniformVariable::LightPosition), positions);
	loadVectors(locations_.at(UniformVariable::LightColor), colors);
	loadVectors(locations_.at(UniformVariable::Attenuation), attenuations);

	loadVector(locations_.at(UniformVariable::SunStrength), scene->getSun()->getColor());

	loadInt(locations_.at(UniformVariable::LightCount), num_lights);
	loadInt(locations_.at(UniformVariable::MaxLights), Light::max_lights);

	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(scene->getCamera());
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(locations_.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));
	 
	// Sky colour
	//loadVector(locations_.at(UniformVariable::FogColor), Application::fog_color);
}

void EntityShader::loadModelMatrix(const Entity& entity) const
{
	// Loading transformation matrix
	glm::mat4 t_matrix = Maths::createTransformationMatrix(entity.getPosition(), 
		entity.getActualRotation(), entity.getScale(), entity.getAlignmentRotation());
	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
}

void EntityShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(locations_.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(locations_.at(UniformVariable::ShineDamper), material.shine_damper);
	loadBoolean(locations_.at(UniformVariable::FakeLighting), material.uses_fake_lighting);
}

