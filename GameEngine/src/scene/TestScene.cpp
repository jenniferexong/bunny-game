#include "TestScene.h"

#include "../text/FontData.h"

// testing loading font data
TestScene::TestScene()
{
	renderable_ = false;
	FontData font_data = FontData("font");
}

