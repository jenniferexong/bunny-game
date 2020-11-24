#include "TextMaster.h"

#include "../Application.h"

void TextMaster::addText(shared_ptr<GuiText> text)
{
	// add to map
	auto font_type = text->getFont();
	if (texts_.find(font_type) == texts_.end()) {
		set<shared_ptr<GuiText>> set;
		texts_.insert({font_type, set });
	}
	texts_.at(font_type).insert(text);
}

void TextMaster::removeText(shared_ptr<GuiText> text)
{
	auto font_type = text->getFont();
	texts_.at(font_type).erase(text);

	if (texts_.at(font_type).empty()) 
		texts_.erase(font_type);
}


