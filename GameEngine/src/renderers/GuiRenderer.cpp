
#include <gl/glew.h>

#include "GuiRenderer.h"

#include <vector>

#include "../Location.h"
#include "../Application.h"
#include "../gui/GuiTexture.h"

GuiRenderer::GuiRenderer()
{
	std::vector<float> positions = {
		-1, 1, -1, -1, 1, 1, 1, -1
	};
	quad_mesh_ = Mesh(Application::loader->loadToVao(positions, 2));
	shader_.setUp();
}

void GuiRenderer::render(const std::unordered_set<std::shared_ptr<GuiTexture>>& gui_textures)
{
	shader_.start();
	glBindVertexArray(quad_mesh_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	// enabling transparency, disable depth test
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	for (const auto& gui: gui_textures) { // render
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gui->getTexture());
		shader_.loadModelMatrix(*gui);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad_mesh_.getVertexCount());
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
}