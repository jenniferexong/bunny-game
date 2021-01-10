#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <string>

#include "../util/JsonValue.h"

class GuiEvent;
class GuiTexture;
class GuiBound;
class GuiText;
class TextMaster;

class GuiComponent {
private:

	GuiComponent* parent_ = nullptr;
    std::shared_ptr<GuiBound> bound_ = nullptr;

    // optional
	std::string id_ = "NONE";
	std::shared_ptr<GuiTexture> gui_ = nullptr;
	std::shared_ptr<GuiText> text_ = nullptr;

	GuiEvent* off_hover_event_;
	GuiEvent* hover_event_;
	GuiEvent* click_event_;

	std::vector<GuiComponent*> components_;

	GuiComponent();

	GuiComponent(
       GuiComponent* parent,
       std::shared_ptr<GuiBound> bound
    );

    GuiComponent* find(const std::string& id);

public:
    // Loading from json file
	GuiComponent(const std::string& file_name);
    void addChildComponent(const Object& component_obj);
    GuiBound makeBound(const Object& bound_obj);
    void setGuiTexture(const Object& gui_obj);
    void setText(const Object& text_obj, const GuiBound& bound);

	~GuiComponent();

    GuiComponent* getComponent(const std::string& id);
	std::weak_ptr<GuiBound> getBound() { return bound_; }
	std::weak_ptr<GuiTexture> getGui() { return gui_; }
	std::weak_ptr<GuiText> getText() { return text_; }

	void setOffHoverEvent(GuiEvent* event);
	void setHoverEvent(GuiEvent* event);
	void setClickEvent(GuiEvent* event);

	void setId(const std::string& id) { id_ = id; }
	const std::string& getId() const { return id_; }

	void setHoverColor(glm::vec4 color);

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
