
#include "GuiTexture.h"

#include "../game-manager/Application.h"
#include "../util/Maths.h"

void GuiTexture::setColor(vec4 color)
{
	color_ = color;
	has_texture_ = false;
}

void GuiTexture::setTexture(int texture)
{
	texture_ = texture;
	has_texture_ = true;
}
