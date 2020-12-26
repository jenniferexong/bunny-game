#pragma once

#include <string>

#include "../shaders/TextShader.h"

class TextMaster;

class TextRenderer {
private:
	static constexpr char name_[] = "TextRenderer";

	TextShader shader_;

	void prepare();
	void renderText(const GuiText& text);
	void finish();

public:
	TextRenderer();
	~TextRenderer() = default;
	void render(const TextMaster& text_master);
};
