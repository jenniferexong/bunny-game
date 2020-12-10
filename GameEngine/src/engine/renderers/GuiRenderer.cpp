
#include <gl/glew.h>

#include "GuiRenderer.h"

#include <vector>

#include "../Location.h"
#include "../Application.h"
#include "../Loader.h"
#include "../../ui/GuiTexture.h"

GuiRenderer::GuiRenderer()
{
	std::vector<float> positions = {
		-1, 1, -1, -1, 1, 1, 1, -1
	};
	quad_mesh_ = Mesh(engine->loader->loadToVao(positions, 2));
	shader_.setUp();
}

void GuiRenderer::render(const std::unordered_set<std::weak_ptr<GuiTexture>>& gui_textures)
{
	shader_.start();
	glBindVertexArray(quad_mesh_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	// enabling transparency, disable depth test
	MasterRenderer::enableAlphaBlending();
	MasterRenderer::disableDepthTest();

	for (const auto& gui: gui_textures) { // render
		if (!gui.lock()->hasTransparency())
			MasterRenderer::disableAlphaBlending();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gui.lock()->getTexture());
		shader_.loadUniforms(*gui.lock());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad_mesh_.getVertexCount());

		if (!gui.lock()->hasTransparency())
			MasterRenderer::enableAlphaBlending();
	}

	MasterRenderer::disableAlphaBlending();
	MasterRenderer::enableDepthTest();

	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
}
