
#include "DefaultShader.h"

#include "../render_engine/MasterRenderer.h"
#include "../Application.h"
#include "../Location.h"

const std::string DefaultShader::s_vertex_file = "res/shaders/default_vert.glsl";
const std::string DefaultShader::s_fragment_file = "res/shaders/default_frag.glsl";

void DefaultShader::setUp()
{
	Shader::setUp(s_vertex_file, s_fragment_file);
}

void DefaultShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "position");
	bindAttribute(AttributeLocation::Normal, "normal");
	bindAttribute(AttributeLocation::Texture, "textureCoords");
}

void DefaultShader::getAllUniformLocations()
{
	m_locations.insert({ UniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	m_locations.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	m_locations.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	m_locations.insert({ UniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	m_locations.insert({ UniformVariable::LightColor, getUniformLocation("uLightColor") });
	m_locations.insert({ UniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	m_locations.insert({ UniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	m_locations.insert({ UniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
	m_locations.insert({ UniformVariable::FakeLighting, getUniformLocation("uFakeLighting") });
	m_locations.insert({ UniformVariable::SkyColor, getUniformLocation("uSkyColor") });
}

void DefaultShader::loadUniformPerFrame(const Light& light) const
{
	// Loading light variables
	loadVector(m_locations.at(UniformVariable::LightPosition), light.getPosition());
	loadVector(m_locations.at(UniformVariable::LightColor), light.getColor());

	// Loading projection matrix
	loadMatrix(m_locations.at(UniformVariable::ProjectionMatrix), MasterRenderer::s_projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::s_camera);
	loadMatrix(m_locations.at(UniformVariable::ViewMatrix), v_matrix);
	loadMatrix(m_locations.at(UniformVariable::InverseViewMatrix), inverse(v_matrix));
	 
	// Sky colour
	loadVector(m_locations.at(UniformVariable::SkyColor), Application::s_sky_color);
}

void DefaultShader::loadModelMatrix(const Entity& entity) const
{
	// Loading transformation matrix
	glm::mat4 t_matrix = Maths::createTransformationMatrix(entity.getPosition(), 
		entity.getRotation(), entity.getScale());
	loadMatrix(m_locations.at(UniformVariable::TransformationMatrix), t_matrix);
}

void DefaultShader::loadMaterial(const Material& material) const
{
	// Loading shine values
	loadFloat(m_locations.at(UniformVariable::Reflectivity), material.reflectivity);
	loadFloat(m_locations.at(UniformVariable::ShineDamper), material.shine_damper);
	loadBoolean(m_locations.at(UniformVariable::FakeLighting), material.uses_fake_lighting);
}

