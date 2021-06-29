#include "FontType.h"

#include "../Engine.h"

FontType::FontType(int texture_atlas, const std::string& font_name):
	texture_atlas_(texture_atlas)
{
	font_data_ = FontData(font_name);
}

FontType::FontType(const std::string& font_name)
{
	// load texture
	texture_atlas_ = Engine::instance->loader->loadFontTexture(font_name);
	font_data_ = FontData(font_name);
}

