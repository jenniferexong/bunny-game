#include <gl/glew.h>

#include "WaterRenderer.h"

#include "../Application.h"
#include "../Location.h"

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

	// load the dudv map
	dudv_id_ = Application::loader->loadTexture("res/textures/water-dudv.png");

	shader_.start();
	shader_.connectTextureUnits();
	shader_.stop();
}

void WaterRenderer::render(const Environment& environment)
{
	prepare(*environment.getCamera());
	for (const auto& water: environment.getWater()) {
		shader_.loadModelMatrix(water);
		glDrawArrays(GL_TRIANGLES, 0, quad_.getVertexCount());
	}
	unbind();
}

void WaterRenderer::prepare(const Camera& camera)
{
	shader_.start();

	// move the water ripples
	move_factor_ += wave_speed * Application::frame_delta;
	move_factor_ = fmod(move_factor_, 1.f);
	shader_.loadUniformPerFrame(camera, move_factor_);

	glBindVertexArray(quad_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::Reflection);
	glBindTexture(GL_TEXTURE_2D, reflection_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::Refraction);
	glBindTexture(GL_TEXTURE_2D, refraction_id_);
	glActiveTexture(GL_TEXTURE0 + WaterTextureLocation::DistortionMap);
	glBindTexture(GL_TEXTURE_2D, dudv_id_);
}

void WaterRenderer::unbind()
{
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);
	shader_.stop();
}





