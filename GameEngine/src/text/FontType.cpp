#include "FontType.h"

FontType::FontType(int texture_atlas, const std::string& font_name) : texture_atlas_(texture_atlas)
{
	//loader_ = TextLoader(font_name);
	font_data_ = FontData(font_name);
}
