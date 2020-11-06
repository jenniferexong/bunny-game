
#include "DefaultShader.h"

#include "../render_engine/MasterRenderer.h"
#include "../Application.h"
#include "../Location.h"

const std::string DefaultShader::vertex_file = "res/shaders/default_vert.glsl";
const std::string DefaultShader::fragment_file = "res/shaders/default_frag.glsl";

void DefaultShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

void DefaultShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "position");
	bindAttribute(AttributeLocation::Normal, "normal");
	bindAttribute(AttributeLocation::Texture, "textureCoords");
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
}

void DefaultShader::loadUniformPerFrame(const Light& light) const
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

