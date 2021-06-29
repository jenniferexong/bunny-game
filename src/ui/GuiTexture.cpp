
#include "GuiTexture.h"

#include "../game-manager/Application.h"
#include "../util/Maths.h"

std::unordered_map<std::string, int> GuiTexture::textures;

void GuiTexture::loadTextures()
{
    textures.insert({"blur-bg", Engine::instance->post_processor->getBlurTexture()});
    textures.insert({"compass", Engine::instance->loader->loadTexture("compass")});
    textures.insert({"cross-hair", Engine::instance->loader->loadTexture("cross-hair")});
}

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
