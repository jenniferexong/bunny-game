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

using text_map = std::unordered_map<std::shared_ptr<FontType>, std::set<std::shared_ptr<GuiText>>>;

class TextMaster {
private:
	text_map texts_;
	
public:
	void addText(std::shared_ptr<GuiText> text);
	void removeText(std::shared_ptr<GuiText> text);
	const text_map& getText() const { return texts_; }
};
