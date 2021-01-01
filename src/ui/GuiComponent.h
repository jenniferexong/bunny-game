#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <string>

class GuiEvent;
class GuiTexture;
class GuiBound;
class GuiText;
class TextMaster;

class GuiComponent {
private:
	GuiComponent* parent_ = nullptr;

	std::string id_ = "";
	std::shared_ptr<GuiTexture> gui_ = nullptr;
	std::shared_ptr<GuiText> text_ = nullptr;

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
	GuiComponent* getComponent(const std::string& id);

	std::weak_ptr<GuiText> addText(
		const std::string& text, float font_size,
		const std::string& font, glm::vec2 position,
		float max_line_width, bool centered
	);

	void setOffHoverEvent(GuiEvent* event);
	void setHoverEvent(GuiEvent* event);
	void setClickEvent(GuiEvent* event);

	void setId(const std::string& id) { id_ = id; }

	void setHoverColor(glm::vec4 color);

	std::weak_ptr<GuiTexture> getGui() { return gui_; }

	void onHover(glm::vec2 mouse_pos);
	void onClick(glm::vec2 mouse_pos);

	/**
	 * Adds GuiTexture and GuiText to rendering batch
	 */
	void addToBatch(
		std::vector<std::weak_ptr<GuiTexture>>& guis,
		TextMaster& text_master
	);
};
