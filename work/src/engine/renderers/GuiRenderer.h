#pragma once

#include <memory>
#include <unordered_set>
#include <string>

#include "../models/Mesh.h"
#include "../shaders/GuiShader.h"

class GuiTexture;

class GuiRenderer {
private:
	static constexpr char name_[] = "GuiRenderer";

	Mesh quad_mesh_;
	GuiShader shader_;

public:
	GuiRenderer();
	~GuiRenderer() = default;
	void render(
		const std::unordered_set<std::weak_ptr<GuiTexture>>& gui_textures
	);
};
