
#include "SkyboxShader.h"

#include "../Location.h"
#include "../Maths.h"
#include "../render_engine/MasterRenderer.h"
#include "../Application.h"

const std::string SkyboxShader::vertex_file = "res/shaders/skybox-vert.glsl";
const std::string SkyboxShader::fragment_file = "res/shaders/skybox-frag.glsl";
const float SkyboxShader::rotate_speed = 0.1f;
vec3 SkyboxShader::ambient_light = vec3(1.f);

void SkyboxShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void SkyboxShader::getAllUniformLocations()
{
	SkyboxShader::locations_.insert({ UniformVariable::ProjectionMatrix, getUniformLocation("uProjectionMatrix") });
	SkyboxShader::locations_.insert({ UniformVariable::ViewMatrix, getUniformLocation("uViewMatrix") });
	SkyboxShader::locations_.insert({ UniformVariable::FogColor, getUniformLocation("uFogColor") });
	SkyboxShader::locations_.insert({ UniformVariable::AmbientLight, getUniformLocation("uAmbientLight") });
	SkyboxShader::locations_.insert({ UniformVariable::SunStrength, getUniformLocation("uSunStrength") });
}

void SkyboxShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}

/* Load projection and view matrix */
void SkyboxShader::loadUniforms() 
{
	// Loading projection matrix
	loadMatrix(locations_.at(UniformVariable::ProjectionMatrix), MasterRenderer::projection_matrix);

	loadVector(locations_.at(UniformVariable::FogColor), Application::fog_color);

	loadVector(locations_.at(UniformVariable::SunStrength), Application::sun.getColor());

	//loadVector(locations_.at(UniformVariable::AmbientLight), ambient_light);

	// View matrix
	glm::mat4 v_matrix = Maths::createSkyViewMatrix(Application::camera);
	current_rotation_ += rotate_speed * Application::frame_delta;
	v_matrix = glm::rotate(v_matrix, current_rotation_, vec3(0, 1.f, 0));
	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
}
