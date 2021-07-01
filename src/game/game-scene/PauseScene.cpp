#include "PauseScene.h"

#include <GLFW/glfw3.h>

#include "../../engine/Application.h"
#include "../../engine/ui/GuiEvent.h"
#include "../../engine/ui/GuiComponent.h"
#include "../../engine/util/Maths.h"
#include "../../engine/util/JsonParser.h"

using glm::vec3;
using glm::vec2;

PauseScene::PauseScene()
{
	Log::init("PauseScene", false);
	setUp();
	Log::init("PauseScene", true);
}

void PauseScene::init()
{
	Log::s("changed to pausescene");
    app->enableCursor(true);
	update_ = true;
}

void PauseScene::setUp()
{
    gui_ = std::make_unique<GuiComponent>("pause.json");
    gui_->getComponent("resume")->setClickEvent(
        new ChangeScene(&game_scene));
    gui_->getComponent("quit")->setClickEvent(new Quit());

    gui_->addToBatch(guis_, text_master_);
}

bool PauseScene::update()
{
	if (!update_)
		return false;

	render();
	update_ = false;
	return true;
}

void PauseScene::render()
{
    app->renderer->prepare(getProjectionMatrix());
    app->renderer->renderGui(guis_);
    app->renderer->renderText(text_master_);
}

void PauseScene::unpause()
{
	app->changeScene(game_scene);
}

void PauseScene::cursorPosCallback(double x, double y)
{
	update_ = true;
	gui_->onHover(vec2(x, y));
}

void PauseScene::keyCallback(int key, int scan_code, int action, int mods)
{
	switch(key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			unpause();
		break;
	case GLFW_KEY_W: 
		if (action == GLFW_PRESS)
			move_keys_[Key::W] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::W] = false;
		break;
	case GLFW_KEY_A: 
		if (action == GLFW_PRESS)
			move_keys_[Key::A] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::A] = false;
		break;
	case GLFW_KEY_S: 
		if (action == GLFW_PRESS)
			move_keys_[Key::S] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::S] = false;
		break;
	case GLFW_KEY_D: 
		if (action == GLFW_PRESS)
			move_keys_[Key::D] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::D] = false;
		break;
	case GLFW_KEY_SPACE: 
		if (action == GLFW_PRESS)
			move_keys_[Key::Space] = true;
		else if(action == GLFW_RELEASE)
			move_keys_[Key::Space] = false;
		break;
	default:
		break;
	}
}

void PauseScene::scrollCallBack(double x_offset, double y_offset)
{
	
}

void PauseScene::mouseButtonCallback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(app->window, &x, &y);
			gui_->onClick(vec2(x, y));
		}
	}
}
