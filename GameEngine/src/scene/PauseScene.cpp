#include "PauseScene.h"

#include <GLFW/glfw3.h>

#include "../game-manager/Application.h"

using glm::vec3;
using glm::vec2;

PauseScene::PauseScene()
{
	setUp();
}

void PauseScene::init()
{
	Print::s("changed to pausescene");
    glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPos(engine->window, engine->window_width/2.0, engine->window_height/2.0);
	update_ = true;
}

void PauseScene::setUp()
{
	auto font = std::make_shared<FontType>("maiandra");
	pause_menu_ = std::make_shared<GuiText>("Resume\n\n\nQuit", 5.f, font, glm::vec2(0.f, 0.3), 1.f, true);
	pause_menu_->setColor(vec3(0));
	pause_menu_->setGlow(vec3(0.901f, 0.886, 0.517));

	text_master_.addText(pause_menu_);

	background_ = std::make_shared<GuiTexture>(engine->post_processor->getBlurTexture(), vec2(0), vec2(1));
	background_->setFlipVertically();
	background_->setTransparency(false);
	guis_.insert(background_);
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
	engine->renderer->prepare(getProjectionMatrix());
	engine->renderer->renderGui(guis_);
	engine->renderer->renderText(text_master_);
}

void PauseScene::unpause()
{
	app->changeScene(app->game_scene);
}

void PauseScene::cursorPosCallback(double x, double y)
{
	
}

void PauseScene::keyCallback(int key, int scan_code, int action, int mods)
{
	switch(key) {
	case GLFW_KEY_TAB:
		if (action == GLFW_RELEASE) {
			unpause();
		}
		break;
	case GLFW_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	default:
		break;
	}
}

void PauseScene::scrollCallBack(double x_offset, double y_offset)
{
	
}

void PauseScene::mouseButtonCallback(int button, int action, int mods)
{
	
}







