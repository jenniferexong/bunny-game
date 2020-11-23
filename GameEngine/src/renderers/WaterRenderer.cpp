#include <gl/glew.h>

#include "WaterRenderer.h"

#include <iostream>


#include "../Application.h"
#include "../Location.h"

const float WaterRenderer::wave_speed = 0.02f;

WaterRenderer::WaterRenderer(const WaterFrameBuffers& fbos)
{
	shader_.setUp();

	// load the quad mesh
	std::vector<float> positions = { // Just x and z vertex positions here, y is set to 0 in v.shader
		-1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1
	};

	quad_ = Application::loader->loadToVao(positions, 2);

	reflection_id_ = fbos.getReflectionTexture();
	refraction_id_ = fbos.getRefractionTexture();
	depth_id_ = fbos.getRefractionDepthTexture();

	// load the dudv map
	dudv_id_ = Application::loader->loadTexture("water-dudv");
	normal_id_ = Application::loader->loadTexture("water-normal");

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

	// move the water ripples
	move_factor_ += wave_speed * Application::frame_delta;
	move_factor_ = fmod(move_factor_, 1.f);
	shader_.loadUniformPerFrame(environment, move_factor_);

	glBindVertexArray(quad_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::Reflection);
	glBindTexture(GL_TEXTURE_2D, reflection_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::Refraction);
	glBindTexture(GL_TEXTURE_2D, refraction_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::DistortionMap);
	glBindTexture(GL_TEXTURE_2D, dudv_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::NormalMap);
	glBindTexture(GL_TEXTURE_2D, normal_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::DepthMap);
	glBindTexture(GL_TEXTURE_2D, depth_id_);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::unbind()
{
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);
	shader_.stop();
}





