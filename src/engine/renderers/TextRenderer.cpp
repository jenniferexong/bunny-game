#include <GL/glew.h>

#include "TextRenderer.h"

#include "MasterRenderer.h"
#include "../Location.h"
#include "../ui/TextMaster.h"
#include "../util/Log.h"

TextRenderer::TextRenderer()
{
	Log::init("TextRenderer", false);
	shader_.setUp();
	Log::init("TextRenderer", true);
}

void TextRenderer::prepare()
{
	MasterRenderer::enableAlphaBlending();
	MasterRenderer::disableDepthTest();
	shader_.start();
}

void TextRenderer::render(const TextMaster& text_master)
{
	prepare();

	const auto& texts = text_master.getText();
	for (const auto& element: texts) {
		auto font_type = element.first;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font_type.lock()->getTexture());

		for (const auto& text : texts.at(font_type))
			renderText(*text.lock());
	}
	finish();
}

void TextRenderer::renderText(const GuiText& text)
{
	glBindVertexArray(text.getVao());
	glEnableVertexAttribArray(TextAttributeLocation::Position);
	glEnableVertexAttribArray(TextAttributeLocation::Texture);
	shader_.loadUniforms(text);
	glDrawArrays(GL_TRIANGLES, 0, text.getVertexCount());
	glDisableVertexAttribArray(TextAttributeLocation::Position);
	glDisableVertexAttribArray(TextAttributeLocation::Texture);
	glBindVertexArray(0);
}

void TextRenderer::finish()
{
	shader_.stop();
	MasterRenderer::disableAlphaBlending();
	MasterRenderer::enableDepthTest();
}

