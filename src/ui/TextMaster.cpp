#include "TextMaster.h"
#include "../util/Log.h"

using std::weak_ptr;
using std::unordered_set;

void TextMaster::addText(weak_ptr<GuiText> text)
{
	// add to map
	auto font_type = text.lock()->getFont();
	if (texts_.find(font_type) == texts_.end()) {
		unordered_set<weak_ptr<GuiText>> set;
		texts_.insert({font_type, set});
	}
	texts_.at(font_type).insert(text);
}

void TextMaster::removeText(weak_ptr<GuiText> text)
{
	auto font_type = text.lock()->getFont();
	texts_.at(font_type).erase(text);

	if (texts_.at(font_type).empty()) 
		texts_.erase(font_type);
}


