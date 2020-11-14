#include <gl/glew.h>

#include "WaterRenderer.h"

#include "../Application.h"
#include "../Location.h"

WaterRenderer::WaterRenderer()
{
	shader_.setUp();

	// load the quad mesh
	std::vector<float> positions = { // Just x and z vertex positions here, y is set to 0 in v.shader
		-1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1
	};

	quad_ = Application::loader->loadToVao(positions, 2);
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
	shader_.loadUniformPerFrame(camera);
	glBindVertexArray(quad_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);
}

void WaterRenderer::unbind()
{
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);
	shader_.stop();
}





