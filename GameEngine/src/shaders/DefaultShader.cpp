
#include "DefaultShader.h"

#include "../render_engine/MasterRenderer.h"
#include "../Maths.h"
#include "../Application.h"

const std::string DefaultShader::s_vertex_file = "res/shaders/default_vert.glsl";
const std::string DefaultShader::s_fragment_file = "res/shaders/default_frag.glsl";

void DefaultShader::setUp()
{
	Shader::setUp(s_vertex_file, s_fragment_file);
}

void DefaultShader::bindAttributes()
{
	bindAttribute(ePosition, "position");
	bindAttribute(eNormal, "normal");
	bindAttribute(eTexture, "textureCoords");
}

void DefaultShader::getAllUniformLocations()
{
	m_locations.insert({ EUniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	m_locations.insert({ EUniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	m_locations.insert({ EUniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	m_locations.insert({ EUniformVariable::InverseViewMatrix, getUniformLocation("uInverseViewMatrix") });
	m_locations.insert({ EUniformVariable::LightColor, getUniformLocation("uLightColor") });
	m_locations.insert({ EUniformVariable::LightPosition, getUniformLocation("uLightPosition") });
	m_locations.insert({ EUniformVariable::Reflectivity, getUniformLocation("uReflectivity") });
	m_locations.insert({ EUniformVariable::ShineDamper, getUniformLocation("uShineDamper") });
}

void DefaultShader::loadLight(const Light& light) const
{
	// Loading light variables
	loadVector(m_locations.at(EUniformVariable::LightPosition), light.getPosition());
	loadVector(m_locations.at(EUniformVariable::LightColor), light.getColor());
}

void DefaultShader::loadViewProjection() const
{
	// Loading projection matrix
	loadMatrix(m_locations.at(EUniformVariable::ProjectionMatrix), MasterRenderer::s_projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createViewMatrix(Application::s_camera);
	loadMatrix(m_locations.at(EUniformVariable::ViewMatrix), v_matrix);
	loadMatrix(m_locations.at(EUniformVariable::InverseViewMatrix), inverse(v_matrix));
}

void DefaultShader::loadModelMatrix(const Entity& entity) const
{
	// Loading transformation matrix
	glm::mat4 t_matrix = Maths::createTransformationMatrix(entity.getPosition(), 
		entity.getRotation(), entity.getScale());
	loadMatrix(m_locations.at(EUniformVariable::TransformationMatrix), t_matrix);
}

void DefaultShader::loadMaterial(const ModelTexture& texture) const
{
	// Loading shine values
	loadFloat(m_locations.at(EUniformVariable::Reflectivity), texture.getReflectivity());
	loadFloat(m_locations.at(EUniformVariable::ShineDamper), texture.getShineDamper());
}

