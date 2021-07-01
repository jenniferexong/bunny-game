#include "GuiEvent.h"

#include "GuiTexture.h"
#include "../Application.h"

void Quit::excecute()
{
	app->closeWindow();
}

void ChangeScene::excecute()
{
	app->changeScene(*scene_);
}

void ChangeColor::excecute()
{
	gui_.lock()->setColor(color_);
}
