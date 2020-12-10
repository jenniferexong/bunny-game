#pragma once

#include <memory>
#include <unordered_set>

#include "../models/Mesh.h"
#include "../shaders/GuiShader.h"

class GuiTexture;

class GuiRenderer {
private:
	Mesh quad_mesh_;
	GuiShader shader_;

public:
	GuiRenderer();
	~GuiRenderer() = default;
	void render(const std::unordered_set<std::weak_ptr<GuiTexture>>& gui_textures);
};