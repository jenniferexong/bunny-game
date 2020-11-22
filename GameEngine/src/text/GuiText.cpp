#include "GuiText.h"

#include "TextMaster.h"

int GuiText::next_id = 0;

GuiText::GuiText(const std::string& text, float font_size, const FontType& font, glm::vec2 position, float max_line_width, bool centered)
{
	// assign id
	id_ = next_id++;
	text_ = text;
	font_size_ = font_size;
	font_ = font;
	position_ = position;
	max_line_width_ = max_line_width;
	centered_ = centered;
}
