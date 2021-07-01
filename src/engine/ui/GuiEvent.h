#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include <memory>

class Scene;
class GuiTexture;

class GuiEvent {
public:
	GuiEvent() = default;
	virtual ~GuiEvent() = default;

	virtual void excecute() {}
};

class Quit: public GuiEvent {
public:
	void excecute() override;
};

class ChangeScene: public GuiEvent {
private:
	std::weak_ptr<Scene>* scene_;

public:
	ChangeScene(std::weak_ptr<Scene>* scene): scene_(scene) {}

	void excecute() override;
};

class ChangeColor: public GuiEvent {
private:
	glm::vec4 color_;
	std::weak_ptr<GuiTexture> gui_;

public:
	ChangeColor(glm::vec4 color, std::weak_ptr<GuiTexture> gui):
		color_(color), gui_(gui)
	{}

	void excecute() override;
};

