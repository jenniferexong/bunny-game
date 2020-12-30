#include "GuiComponent.h"

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
}

GuiComponent::GuiComponent()
{
	parent_ = nullptr;
	gui_ = std::make_shared<GuiTexture>(vec4(0), GuiBound(vec2(0), vec2(1)));
}

GuiComponent::GuiComponent(GuiComponent* parent, std::shared_ptr<GuiTexture> gui):
	parent_(parent),
	gui_(gui)
{}

/**
 * @param bound position and size in relation 
 * to the size and position of this GuiComponent
 */
std::weak_ptr<GuiTexture> GuiComponent::addComponent(int texture, GuiBound bound) 
{
	vec2 pos = bound.getPosition();
	vec2 scale = bound.getScale();

	vec2 new_pos = gui_->getPosition() + (gui_->getScale() * pos);
	Log::vector(new_pos);
	vec2 new_scale = scale / gui_->getScale();

	std::shared_ptr<GuiTexture> gui = std::make_shared<GuiTexture>(
		texture,
		GuiBound(new_pos, new_scale)
	);

	components_.push_back(new GuiComponent(this, gui));
	return gui;
}

/**
 * @param bound position and size in relation 
 * to the size and position of this GuiComponent
 */
std::weak_ptr<GuiTexture> GuiComponent::addComponent(glm::vec4 color, GuiBound bound)
{
	vec2 pos = bound.getPosition();
	vec2 scale = bound.getScale();

	vec2 new_pos = gui_->getPosition() + dot(scale, (pos / 2.f));
	vec2 new_scale = scale / gui_->getScale();

	std::shared_ptr<GuiTexture> gui = std::make_shared<GuiTexture>(
		color,
		GuiBound(new_pos, new_scale)
	);

	components_.push_back(new GuiComponent(this, gui));
	return gui;
}

// TODO
void GuiComponent::onHover(vec2 mouse_pos) 
{
	if (!gui_->getBounds().containsPoint(mouse_pos))
		return;

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

void GuiComponent::setHoverEvent(GuiEvent* event) 
{
	hover_event_ = event;
}

void GuiComponent::setClickEvent(GuiEvent* event) 
{
	click_event_ = event;
}
