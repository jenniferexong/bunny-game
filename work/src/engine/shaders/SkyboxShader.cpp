
#include "SkyboxShader.h"

#include "../Location.h"
#include "../Utility.h"
#include "../renderers/MasterRenderer.h"
#include "../Application.h"

void SkyboxShader::bindAttributes()
{
	bindAttribute(AttributeLocation::Position, "aPosition");
}

void SkyboxShader::getAllUniformLocations()
{
	INSERT_LOC(ProjectionMatrix, "uProjectionMatrix");
	INSERT_LOC(ViewMatrix, "uViewMatrix");
	INSERT_LOC(DayTexture, "uDayTexture");
	INSERT_LOC(NightTexture, "uNightTexture");
	INSERT_LOC(BlendFactor, "uBlendFactor");
}

void SkyboxShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
	Error::gl_check(name);
}

void SkyboxShader::loadBlendFactor(float blend)
{
	loadFloat(locations_.at(UniformVariable::BlendFactor), blend);
}

void SkyboxShader::connectTextureUnits()
{
	loadInt(locations_.at(UniformVariable::DayTexture), CubeMapLocation::Day);
	loadInt(
		locations_.at(UniformVariable::NightTexture),
		CubeMapLocation::Night
	);
}

/* Load projection and view matrix */
void SkyboxShader::loadUniforms(const Camera& camera) 
{
	// Loading projection matrix
	loadMatrix(
		locations_.at(UniformVariable::ProjectionMatrix),
		MasterRenderer::projection_matrix
	);

	// View matrix
	glm::mat4 v_matrix = Maths::createSkyViewMatrix(camera);
	current_rotation_ += rotate_speed * app->timer->frame_delta;

	loadMatrix(locations_.at(UniformVariable::ViewMatrix), v_matrix);
}
