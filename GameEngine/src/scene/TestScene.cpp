#include "TestScene.h"

#include <memory>

#include "../Application.h"
#include "../text/GuiText.h"

// testing loading font data
TestScene::TestScene(shared_ptr<MasterRenderer> renderer)
{
	renderer_ = std::move(renderer);

	auto font = std::make_shared<FontType>("maiandra");
	text_ = std::make_shared<GuiText>("Hello\nmy name is\nJENNIFER!!", 6.f, font, glm::vec2(0.2f, 0.25), 1.f, false);
	text_->setOutline(vec3(0, 1.f, 1));
	auto glow = std::make_shared<GuiText>("Hello\nmy name is\nJENNIFER!!", 6.f, font, glm::vec2(0.5f, 0.25), 1.f, false);
	glow->setColor(vec3(1.f));
	glow->setGlow(vec3(0, 0.f, 0));

	text_master_.addText(text_);
	text_master_.addText(glow);
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
	//text_->updateText(text);
}


