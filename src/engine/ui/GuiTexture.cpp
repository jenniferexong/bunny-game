
#include "GuiTexture.h"

#include "../Application.h"
#include "../util/Maths.h"

std::unordered_map<std::string, int> GuiTexture::textures_;

void GuiTexture::loadTextures(std::unordered_map<std::string, int> textures)
{
    textures_ = textures;
    //textures.insert({"blur-bg", app->post_processor->getBlurTexture()});
    //textures.insert({"compass", app->loader->loadTexture("compass.png")});
    //textures.insert({"cross-hair", app->loader->loadTexture("cross-hair.png")});
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
