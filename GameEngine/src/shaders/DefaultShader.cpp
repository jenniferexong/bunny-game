
#include "DefaultShader.h"

#include "../AttributeLocation.h"
#include "../Maths.h"
#include "../Entity.h"
#include "../Application.h"
#include "../Light.h"

const std::string DefaultShader::s_vertex_file = "res/shaders/default_vert.glsl";
const std::string DefaultShader::s_fragment_file = "res/shaders/default_frag.glsl";

void DefaultShader::setUp() {
	Shader::setUp(s_vertex_file, s_fragment_file);
}

void DefaultShader::bindAttributes() {
	bindAttribute(ePosition, "position");
	bindAttribute(eNormal, "normal");
	bindAttribute(eTexture, "textureCoords");
}

void DefaultShader::getAllUniformLocations() {
	m_locations.insert({ EUniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	m_locations.insert({ EUniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	m_locations.insert({ EUniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	m_locations.insert({ EUniformVariable::LightColor, getUniformLocation("uLightColor") });
	m_locations.insert({ EUniformVariable::LightPosition, getUniformLocation("uLightPosition") });
}

void DefaultShader::loadAllUniforms(const Entity& entity) const {
	// Loading projection matrix
	loadMatrix(m_locations.at(EUniformVariable::ProjectionMatrix), Renderer::s_projection_matrix);

	// Loading transformation matrix
	glm::mat4 t_matrix = Maths::createTransformationMatrix(entity.getPosition(), 
		entity.getRotation(), entity.getScale());
	loadMatrix(m_locations.at(EUniformVariable::TransformationMatrix), t_matrix);

	glm::mat4 v_matrix = Maths::createViewMatrix(Application::s_camera);
	loadMatrix(m_locations.at(EUniformVariable::ViewMatrix), v_matrix);

	// Loading light variables
	loadVector(m_locations.at(EUniformVariable::LightPosition), Application::s_light.getPosition());
	loadVector(m_locations.at(EUniformVariable::LightColor), Application::s_light.getColor());
}

