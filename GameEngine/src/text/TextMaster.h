#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "GuiText.h"
#include "FontType.h"

class GuiText;

namespace std
{
	template <>
	struct hash<weak_ptr<FontType>>
	{
		size_t operator()(const weak_ptr<FontType>& font_type) const
		{
			return hash<int>()(font_type.lock()->getTexture());
		}
	};
	template<>
	struct hash<weak_ptr<GuiText>>
	{
		size_t operator()(const weak_ptr<GuiText>& text) const
		{
			return hash<int>()(text.lock()->getVao());
		}
	};
	template<>
	struct equal_to<weak_ptr<FontType>>
	{
		bool operator() (const weak_ptr<FontType>& a, const weak_ptr<FontType>& b) const
		{
			return a.lock() == b.lock();
		}
	};
	template<>
	struct equal_to<weak_ptr<GuiText>>
	{
		bool operator() (const weak_ptr<GuiText>& a, const weak_ptr<GuiText>& b) const 
		{
			return a.lock() == b.lock();
		}
	};
}

using text_map = std::unordered_map<std::weak_ptr<FontType>, std::unordered_set<std::weak_ptr<GuiText>>>;

class TextMaster {
private:
	text_map texts_;
	
public:
	void addText(std::weak_ptr<GuiText> text);
	void removeText(std::weak_ptr<GuiText> text);
	const text_map& getText() const { return texts_; }
};
