#include "TestScene.h"

#include <memory>

#include "../Application.h"
#include "../text/GuiText.h"

// testing loading font data
TestScene::TestScene(shared_ptr<MasterRenderer> renderer)
{
	renderer_ = std::move(renderer);

	auto font = std::make_shared<FontType>("font");
	text_ = std::make_shared<GuiText>("Hello\nmy name is\nJENNIFER!!", 1.f, font, glm::vec2(0.5), 0.25f, false);

	text_master_.addText(text_);
}

void TestScene::render()
{
	renderer_->updateWindowSize();
	renderer_->prepare(getProjectionMatrix());
	renderer_->renderText(text_master_);
}

void TestScene::keyCallback(int key, int scan_code, int action, int mods)
{
	string text = string(1, char(key));
	text_->updateText(text);
}


