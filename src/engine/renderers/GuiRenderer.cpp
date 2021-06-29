#include <GL/glew.h>

#include "GuiRenderer.h"

#include <vector>

#include "../Location.h"
#include "../Engine.h"
#include "../Loader.h"
#include "../../ui/GuiTexture.h"
#include "../Utility.h"

GuiRenderer::GuiRenderer()
{
	Log::init("GuiRenderer", false);

	std::vector<float> positions = {
		-1, 1, -1, -1, 1, 1, 1, -1 
	};

	quad_mesh_ = Mesh(Engine::instance->loader->loadToVao(positions, 2));
	shader_.setUp();

	Log::init("GuiRenderer", true);
}

void GuiRenderer::render(
	const std::vector<std::weak_ptr<GuiTexture>>& gui_textures)
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

		if (gui.lock()->hasTexture()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gui.lock()->getTexture());
		}
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

	Error::gl_check(name_);
}
