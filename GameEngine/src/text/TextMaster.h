#pragma once

#include <unordered_map>
#include <set>

#include "FontType.h"

class GuiText;


namespace std
{
	template <>
	struct hash<FontType>
	{
		size_t operator()(const FontType& font_type) const
		{
			return hash<int>()(font_type.getTexture());
		}
	};
}

/**
 * Stores all of the text.
 */
class TextMaster {
private:
	std::unordered_map<FontType, std::set<GuiText>> texts_; // TODO: may have to provide hash and equality here?
	
public:
	void addText(const GuiText& text);
	void removeText(const GuiText& text);
};
