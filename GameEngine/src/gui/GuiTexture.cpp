
#include "GuiTexture.h"

#include "../Application.h"

/* Calculates position and scale in percentage of the window size, from pixel coordinates (+x is right, +y is up) */
GuiTexture::GuiTexture(int texture, glm::vec2 position, glm::vec2 size) : texture_(texture)
{
	int width, height;
	glfwGetWindowSize(Application::window, &width, &height);

	glm::vec2 pos = position - vec2((float)width / 2, (float)height / 2);
	pos.x /= width;
	pos.y /= height;

	position_ = 2.f * pos;
	scale_ = vec2(size.x / width, size.y / height);
}
