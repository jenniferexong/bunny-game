#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class GuiEvent;
class GuiTexture;
class GuiBound;

class GuiComponent {
private:
	GuiComponent* parent_ = nullptr;

	std::shared_ptr<GuiTexture> gui_;

	GuiEvent* off_hover_event_;
	GuiEvent* hover_event_;
	GuiEvent* click_event_;

	std::vector<GuiComponent*> components_;

public:
	GuiComponent();
	GuiComponent(GuiComponent* parent, std::shared_ptr<GuiTexture> gui);
	~GuiComponent();

	GuiComponent* addComponent(int texture, GuiBound bound);
	GuiComponent* addComponent(glm::vec4 color, GuiBound bound);

	void setOffHoverEvent(GuiEvent* event);
	void setHoverEvent(GuiEvent* event);
	void setClickEvent(GuiEvent* event);

	void setHoverColor(glm::vec4 color);

	std::weak_ptr<GuiTexture> getGui() { return gui_; }

	void onHover(glm::vec2 mouse_pos);
	void onClick(glm::vec2 mouse_pos);

	void addToGuis(std::vector<std::weak_ptr<GuiTexture>>& guis);
};
