#include "GuiEvent.h"

#include "GuiTexture.h"
#include "../../game/game-manager/Application.h"
#include "../Engine.h"

void Quit::excecute()
{
    Engine::instance->closeWindow();
}

void ChangeScene::excecute() 
{
	app->changeScene(scene_);
}

void ChangeColor::excecute()
{
	gui_.lock()->setColor(color_);
}

