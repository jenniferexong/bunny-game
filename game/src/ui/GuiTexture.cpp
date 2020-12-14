
#include "GuiTexture.h"

#include "../game-manager/Application.h"

/**
 * Calculates position and scale in percentage of the window size, 
 * from pixel coordinates (+x is right, +y is up)
 */
GuiTexture::GuiTexture(int texture, glm::ivec2 position, glm::ivec2 size): 
	texture_(texture)
{
	auto width = (float) engine->window_width;
	auto height = (float) engine->window_height;

	glm::vec2 pos = glm::vec2((float)position.x, position.y) 
		- glm::vec2(width / 2, height / 2);
	pos.x /= width;
	pos.y /= height;

	position_ = 2.f * pos;
	scale_ = glm::vec2((float)size.x / width, (float)size.y / height);
}
