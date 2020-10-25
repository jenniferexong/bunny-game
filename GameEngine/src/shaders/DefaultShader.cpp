
#include "DefaultShader.h"

#include "../AttributeLocation.h"
#include "../Maths.h"
#include "../Entity.h"

const std::string DefaultShader::s_vertex_file = "res/shaders/default_vert.glsl";
const std::string DefaultShader::s_fragment_file = "res/shaders/default_frag.glsl";

void DefaultShader::setUp() {
	Shader::setUp(s_vertex_file, s_fragment_file);
}

void DefaultShader::bindAttributes() {
	bindAttribute(ePosition, "position");
	bindAttribute(eTexture, "textureCoords");
}

void DefaultShader::getAllUniformLocations() {
	m_locations.insert({ EUniformVariable::TransformationMatrix, getUniformLocation("uTransformationMatrix") });
	m_locations.insert({ EUniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
}

void DefaultShader::loadAllUniforms(const glm::mat4 projection, const Entity& entity) const {
	// Loading projection matrix
	loadMatrix(m_locations.at(EUniformVariable::ProjectionMatrix), projection);

	// Loading transformation matrix
	glm::mat4 matrix = Maths::createTransformationMatrix(entity.getPosition(), 
		entity.getRotation(), entity.getScale());
	loadMatrix(m_locations.at(EUniformVariable::TransformationMatrix), matrix);
}

