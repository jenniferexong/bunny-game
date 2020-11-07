
#include <gl/glew.h>

#include "GuiRenderer.h"

#include <vector>

#include "../Location.h"
#include "../Application.h"
#include "../gui/GuiTexture.h"

Mesh GuiRenderer::quad_mesh = Mesh();

GuiRenderer::GuiRenderer()
{
	std::vector<float> positions = {
		-1, 1, -1, -1, 1, 1, 1, -1
	};
	quad_mesh = Mesh(Application::loader.loadToVao(positions));
	shader_.setUp();
}

void GuiRenderer::render(const std::vector<std::shared_ptr<GuiTexture>>& gui_textures)
{
	shader_.start();
	glBindVertexArray(quad_mesh.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	// enabling transparency, disable depth test
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	for (const auto gui: gui_textures) { // render
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gui->getTexture());
		shader_.loadModelMatrix(*gui);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad_mesh.getVertexCount());
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
}