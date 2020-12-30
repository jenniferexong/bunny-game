#include "GuiEvent.h"
#include "../game-manager/Application.h"

void UnpauseEvent::excecute() 
{
	app->changeScene(app->game_scene);
}
