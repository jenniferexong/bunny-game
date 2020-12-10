#include <gl/glew.h>

#include "WaterRenderer.h"

#include "../Application.h"
#include "../Utility.h"
#include "../Environment.h"
#include "../Location.h"


WaterRenderer::WaterRenderer()
{
	shader_.setUp();

	// load the quad mesh
	std::vector<float> positions = { // Just x and z vertex positions here, y is set to 0 in v.shader
		-1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1
	};

	quad_ = engine->loader->loadToVao(positions, 2);

	// load the dudv map
	dudv_id_ = engine->loader->loadTexture("water-dudv");
	normal_id_ = engine->loader->loadTexture("water-normal");

	shader_.start();
	shader_.connectTextureUnits();
	shader_.stop();
}

void WaterRenderer::render(const Environment& environment)
{
	if (environment.getWater().empty())
		return;

	prepare(environment);
	for (const auto& water: environment.getWater()) {
		shader_.loadModelMatrix(water);
		glDrawArrays(GL_TRIANGLES, 0, quad_.getVertexCount());
	}
	unbind();
}

void WaterRenderer::prepare(const Environment& environment)
{
	shader_.start();

	shader_.loadUniformPerFrame(environment);

	glBindVertexArray(quad_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::Reflection);
	glBindTexture(GL_TEXTURE_2D, reflection_fbo_.getColorTexture());
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::Refraction);
	glBindTexture(GL_TEXTURE_2D, refraction_fbo_.getColorTexture());
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::DistortionMap);
	glBindTexture(GL_TEXTURE_2D, dudv_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::NormalMap);
	glBindTexture(GL_TEXTURE_2D, normal_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::DepthMap);
	glBindTexture(GL_TEXTURE_2D, refraction_fbo_.getDepthTexture());

	MasterRenderer::enableAlphaBlending();
}

void WaterRenderer::unbind()
{
	MasterRenderer::disableAlphaBlending();
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);
	shader_.stop();
}





