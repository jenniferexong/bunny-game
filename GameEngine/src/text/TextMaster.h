#pragma once

#include <unordered_map>
#include <set>

#include "FontType.h"
#include "TextLoader.h"

class GuiText;

namespace std
{
	template <>
	struct hash<shared_ptr<FontType>>
	{
		size_t operator()(const shared_ptr<FontType>& font_type) const
		{
			return hash<int>()(font_type->getTexture());
		}
	};
}

using text_map = std::unordered_map<std::shared_ptr<FontType>, std::set<GuiText>>;

class TextMaster {
private:
	TextLoader loader_;
	text_map texts_; // TODO: may have to provide hash and equality here?
	
public:
	void addText(GuiText& text);
	void removeText(const GuiText& text);
	const text_map& getText() const { return texts_; }
};
