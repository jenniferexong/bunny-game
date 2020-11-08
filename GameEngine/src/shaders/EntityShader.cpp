
#include "EntityShader.h"

#include "../render_engine/MasterRenderer.h"
#include "../Application.h"
#include "../Location.h"

const std::string DefaultShader::vertex_file = "res/shaders/entity_vert.glsl";
const std::string DefaultShader::fragment_file = "res/shaders/entity_frag.glsl";

void DefaultShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

void DefaultShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
	bindAttribute(AttributeLocation::Normal, "aNormal");
	bindAttribute(AttributeLocation::Texture, "aTextureCoords");
}

void DefaultShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	locations_.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	locations_.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	locations_.insert({ UniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	locations_.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	locations_.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	locations_.insert({ UniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	locations_.insert({ UniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	locations_.insert({ UniformVariable::FakeLighting, getUniformLocation("uFakeLighting") });
	locations_.insert({ UniformVariable::SkyColor, getUniformLocation("uSkyColor") });
	locations_.insert({ UniformVariable::LightCount, getUniformLocation("uLightCount") });
	locations_.insert({ UniformVariable::MaxLights, getUniformLocation("uMaxLights") });
}

void DefaultShader::loadUniformPerFrame(const std::vector<Light>& lights) const
{
	// Loading light variables
	int num_lights = lights.size();
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	positions.reserve(num_lights);
	colors.reserve(num_lights);
	for (const auto& l: lights) {
		positions.emplace_back(l.getPosition());
		colors.emplace_back(l.getColor());
	}

	loadVectors(locations_.at(UniformVariable::LightPosition), positions);
	loadVectors(locations_.at(UniformVariable::LightColor), colors);
	loadInt(locations_.at(UniformVariable::LightCount), num_lights);
	loadInt(locations_.at(UniformVariable::MaxLights), Light::max_lights);

	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::camera);
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(locations_.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));
	 
	// Sky colour
	loadVector(locations_.at(UniformVariable::SkyColor), Application::sky_color);
}

void DefaultShader::loadModelMatrix(const Entity& entity) const
{
	// Loading transformation matrix
	glm::mat4 t_matrix = Maths::createTransformationMatrix(entity.getPosition(), 
		entity.getActualRotation(), entity.getScale(), entity.getAlignmentRotation());
	loadMatrix(locations_.at(UniformVariable::TransformationMatrix), t_matrix);
}

void DefaultShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(locations_.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(locations_.at(UniformVariable::ShineDamper), material.shine_damper);
	loadBoolean(locations_.at(UniformVariable::FakeLighting), material.uses_fake_lighting);
}

