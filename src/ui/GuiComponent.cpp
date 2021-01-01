#include "GuiComponent.h"

#include "GuiEvent.h"
#include "GuiTexture.h"
#include "GuiText.h"
#include "GuiBound.h"
#include "TextMaster.h"
#include "../util/Log.h" 

using namespace glm;

GuiComponent::~GuiComponent()
{
	for (const auto& c: components_)
		delete c;

	delete hover_event_;
	delete click_event_;
	delete off_hover_event_;
}

/**
 * Creates a base GuiComponent which is the size of the 
 * entire window, and completely transparent
 */
GuiComponent::GuiComponent()
{
	parent_ = nullptr;
	gui_ = std::make_shared<GuiTexture>(vec4(0), GuiBound(vec2(0.5), vec2(1)));

	hover_event_ = new GuiEvent();
	off_hover_event_ = new GuiEvent();
	click_event_ = new GuiEvent();
}

GuiComponent::GuiComponent(GuiComponent* parent, std::shared_ptr<GuiTexture> gui):
	parent_(parent),
	gui_(gui)
{
	hover_event_ = new GuiEvent();
	off_hover_event_ = new GuiEvent();
	click_event_ = new GuiEvent();
}

/**
 * @param bound position and size in relation 
 * to the size and position of this GuiComponent
 */
GuiComponent* GuiComponent::addComponent(int texture, GuiBound bound) 
{
	std::shared_ptr<GuiTexture> gui = std::make_shared<GuiTexture>(
		texture,
		GuiBound(bound, gui_->getBound())
	);

	GuiComponent* new_component = new GuiComponent(this, gui);
	components_.push_back(new_component);
	return new_component;
}

/**
 * @param bound position and size in relation 
 * to the size and position of this GuiComponent
 */
GuiComponent* GuiComponent::addComponent(glm::vec4 color, GuiBound bound)
{
	std::shared_ptr<GuiTexture> gui = std::make_shared<GuiTexture>(
		color,
		GuiBound(bound, gui_->getBound())
	);
	GuiComponent* new_component = new GuiComponent(this, gui);
	components_.push_back(new_component);
	return new_component;
}

GuiComponent* GuiComponent::getComponent(const std::string& id)
{
	if (id_ == id)
		return this;

	for (auto& c: components_) {
		GuiComponent* found = c->getComponent(id);
		if (found != nullptr)
			return found;
	}

	return nullptr;
}

std::weak_ptr<GuiText> GuiComponent::addText(
	const std::string& text, float font_size,
	const std::string& font, glm::vec2 position,
	float max_line_width, bool centered)
{
	text_ = std::make_shared<GuiText>(
		text, font_size, font, position,
		max_line_width, centered, gui_->getBound()
	);
	return text_;
}

// TODO
void GuiComponent::onHover(vec2 mouse_pos) 
{
	if (!gui_->getBound().containsPoint(mouse_pos))
		off_hover_event_->excecute();
	else 
		hover_event_->excecute();

	for (const auto& c: components_)
		c->onHover(mouse_pos);
}

/**
 * Adds GuiTexture and GuiText to rendering batch
 */
void GuiComponent::addToBatch(
	std::vector<std::weak_ptr<GuiTexture>>& guis,
	TextMaster& text_master)
{
	if (gui_ != nullptr)
		guis.push_back(gui_);
	if (text_ != nullptr)
		text_master.addText(text_);

	for (const auto& c: components_)
		c->addToBatch(guis, text_master);
}

void GuiComponent::onClick(vec2 mouse_pos) 
{
	if (!gui_->getBound().containsPoint(mouse_pos))
		return;

	click_event_->excecute();

	for (const auto& c: components_)
		c->onClick(mouse_pos);
}

void GuiComponent::setHoverColor(vec4 color) 
{
	setHoverEvent(new ChangeColor(color, gui_));
	setOffHoverEvent(new ChangeColor(gui_->getColor(), gui_));
}

void GuiComponent::setHoverEvent(GuiEvent* event) 
{
	hover_event_ = event;
}

void GuiComponent::setOffHoverEvent(GuiEvent* event) 
{
	off_hover_event_ = event;
}

void GuiComponent::setClickEvent(GuiEvent* event) 
{
	click_event_ = event;
}
