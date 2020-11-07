#pragma once

#include <vector>
#include <memory>

#include "../models/Mesh.h"
#include "../shaders/GuiShader.h"

class GuiTexture;

class GuiRenderer {
private:
	static Mesh quad_mesh;

	GuiShader shader_;

public:
	GuiRenderer();
	void render(const std::vector<std::shared_ptr<GuiTexture>>& gui_textures);
};
