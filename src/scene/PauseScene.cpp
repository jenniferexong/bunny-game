#include "PauseScene.h"

#include <GLFW/glfw3.h>

#include "../game-manager/Application.h"
#include "../util/Maths.h"

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
	engine->enableCursor(true);
	update_ = true;
}

void PauseScene::setUp()
{
	auto font = std::make_shared<FontType>("maiandra");
	pause_menu_ = std::make_shared<GuiText>(
		"Resume\n\n\nQuit", 5.f, font, glm::vec2(0.f, 0.3), 1.f, true);
	pause_menu_->setColor(vec3(0));
	pause_menu_->setGlow(vec3(0.901f, 0.886, 0.517));

	text_master_.addText(pause_menu_);

	background_ = std::make_shared<GuiTexture>(
		engine->post_processor->getBlurTexture(), 
		GuiBound(vec2(0), vec2(1))
	);
	background_->setFlipVertically();
	background_->setTransparency(false);
	guis_.push_back(background_);
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
	//Log::vector(vec2(x, y));
	//Log::vector(Maths::pixelToScreenCoords(vec2(x, y)));
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
		engine->closeWindow();
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
