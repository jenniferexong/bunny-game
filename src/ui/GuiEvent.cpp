#include "GuiEvent.h"

#include "GuiTexture.h"
#include "../game-manager/Application.h"

void ChangeScene::excecute() 
{
	app->changeScene(scene_);
}

void ChangeColor::excecute()
{
	gui_.lock()->setColor(color_);
}
