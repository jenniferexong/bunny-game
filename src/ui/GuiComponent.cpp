#include "GuiComponent.h"

#include "GuiEvent.h"
#include "GuiTexture.h"
#include "GuiBound.h"
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

GuiComponent::GuiComponent()
{
	parent_ = nullptr;
	gui_ = std::make_shared<GuiTexture>(vec4(0), GuiBound(vec2(0), vec2(1)));

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
	vec2 pos = bound.getPosition();
	vec2 scale = bound.getScale();

	vec2 new_pos = gui_->getPosition() + (gui_->getScale() * pos);
	vec2 new_scale = scale / gui_->getScale();

	std::shared_ptr<GuiTexture> gui = std::make_shared<GuiTexture>(
		texture,
		GuiBound(new_pos, new_scale)
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
	vec2 pos = bound.getPosition();
	vec2 scale = bound.getScale();

	vec2 new_pos = gui_->getPosition() + (gui_->getScale() * pos);
	vec2 new_scale = scale / gui_->getScale();

	std::shared_ptr<GuiTexture> gui = std::make_shared<GuiTexture>(
		color,
		GuiBound(new_pos, new_scale)
	);
	GuiComponent* new_component = new GuiComponent(this, gui);
	components_.push_back(new_component);
	return new_component;
}

// TODO
void GuiComponent::onHover(vec2 mouse_pos) 
{
	if (!gui_->getBounds().containsPoint(mouse_pos))
		off_hover_event_->excecute();
	else 
		hover_event_->excecute();

	for (const auto& c: components_)
		c->onHover(mouse_pos);
}

void GuiComponent::addToGuis(std::vector<std::weak_ptr<GuiTexture>>& guis)
{
	guis.push_back(gui_);
	for (const auto& c: components_)
		c->addToGuis(guis);
}

void GuiComponent::onClick(vec2 mouse_pos) 
{
	if (!gui_->getBounds().containsPoint(mouse_pos))
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
