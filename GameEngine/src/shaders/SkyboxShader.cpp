
#include "SkyboxShader.h"

#include "../Location.h"
#include "../Maths.h"
#include "../render_engine/MasterRenderer.h"
#include "../Application.h"

const std::string SkyboxShader::vertex_file = "res/shaders/skybox-vert.glsl";
const std::string SkyboxShader::fragment_file = "res/shaders/skybox-frag.glsl";

void SkyboxShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void SkyboxShader::getAllUniformLocations()
{
	SkyboxShader::locations_.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	SkyboxShader::locations_.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
}

void SkyboxShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

/* Load projection and view matrix */
void SkyboxShader::loadUniforms() const
{
	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	// View matrix
	glm::mat4 v_matrix = Maths::createSkyViewMatrix(Application::camera);
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
}
