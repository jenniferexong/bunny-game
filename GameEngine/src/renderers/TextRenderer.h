#pragma once
#include "../shaders/TextShader.h"

class TextMaster;

class TextRenderer {
private:
	TextShader shader_;

	void prepare();
	void renderText(const GuiText& text);
	void finish();

public:
	TextRenderer();
	~TextRenderer() = default;
	void render(const TextMaster& text_master);
};
