
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
	_locations.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	_locations.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	_locations.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	_locations.insert({ UniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	_locations.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	_locations.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	_locations.insert({ UniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	_locations.insert({ UniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	_locations.insert({ UniformVariable::FakeLighting, getUniformLocation("uFakeLighting") });
	_locations.insert({ UniformVariable::SkyColor, getUniformLocation("uSkyColor") });
}

void DefaultShader::loadUniformPerFrame(const Light& light) const
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
	 
	// Sky colour
	loadVector(_locations.at(UniformVariable::SkyColor), Application::sky_color);
}

void DefaultShader::loadModelMatrix(const Entity& entity) const
{
	// Loading transformation matrix
	glm::mat4 t_matrix = Maths::createTransformationMatrix(entity.getPosition(), 
		entity.getRotation(), entity.getScale());
	loadMatrix(_locations.at(UniformVariable::TransformationMatrix), t_matrix);
}

void DefaultShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(_locations.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(_locations.at(UniformVariable::ShineDamper), material.shine_damper);
	loadBoolean(_locations.at(UniformVariable::FakeLighting), material.uses_fake_lighting);
}

