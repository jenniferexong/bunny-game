#include "TestScene.h"

#include <memory>

#include "../Application.h"
#include "../text/FontData.h"
#include "../text/GuiText.h"

// testing loading font data
TestScene::TestScene(shared_ptr<MasterRenderer> renderer)
{
	renderable_ = false;
	renderer_ = std::move(renderer);

	auto font = std::make_shared<FontType>("font");
	GuiText text = GuiText("Hello my name is Jennifer and Nibbles is the cutest bunny in the entire world!!!!!!",
		1, font, glm::vec2(0.5), 0.25f, false);

	text_master_.addText(text);
}

void TestScene::render()
{
	renderer_->updateWindowSize();
	renderer_->prepare(getProjectionMatrix());
	renderer_->renderText(text_master_);
}


