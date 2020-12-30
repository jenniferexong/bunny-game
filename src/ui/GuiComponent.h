#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "GuiEvent.h"

class GuiTexture;
class GuiBound;

class GuiComponent {
private:
	GuiComponent* parent_ = nullptr;

	std::shared_ptr<GuiTexture> gui_;
	GuiEvent* hover_event_ = new GuiEvent();
	GuiEvent* click_event_ = new GuiEvent();

	std::vector<GuiComponent*> components_;

public:
	GuiComponent();
	GuiComponent(GuiComponent* parent, std::shared_ptr<GuiTexture> gui);

	std::weak_ptr<GuiTexture> addComponent(int texture, GuiBound bound);
	std::weak_ptr<GuiTexture> addComponent(glm::vec4 color, GuiBound bound);

	void setHoverEvent(GuiEvent* event);
	void setClickEvent(GuiEvent* event);

	std::weak_ptr<GuiTexture> getGui() { return gui_; }

	void onHover(glm::vec2 mouse_pos);
	void onClick(glm::vec2 mouse_pos);

	void addToGuis(std::vector<std::weak_ptr<GuiTexture>>& guis);

	~GuiComponent();
};
