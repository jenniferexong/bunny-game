#include "TestScene.h"

#include <memory>

#include "../Application.h"
#include "../text/FontData.h"
#include "../text/GuiText.h"

// testing loading font data
TestScene::TestScene(shared_ptr<MasterRenderer> renderer)
{
	renderable_ = false;
	renderer_ = renderer;

	auto font = std::make_shared<FontType>("font");
	GuiText text = GuiText("Hello my name is Jennifer and I really eating cucumber. Also Nibbles is a very cute bunny.",
		10, font, glm::vec2(0.5f), 0.25f, true);

	text_.addText(text);
}

void TestScene::render()
{
}


