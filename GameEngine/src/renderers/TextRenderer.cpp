#include <GL/glew.h>

#include "TextRenderer.h"

#include "../Location.h"
#include "../text/TextMaster.h"

TextRenderer::TextRenderer()
{
	shader_.setUp();
}

void TextRenderer::prepare()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
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
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

