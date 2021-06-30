#pragma once

#include <unordered_map>

#include "Shader.h"

class GuiTexture;

class GuiShader final : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, FlipVertically,
		HasTexture, Color
	};

	static constexpr char name[] = "GuiShader";
	static constexpr char vertex_file[] = "gui-vert.glsl";
	static constexpr char fragment_file[] = "gui-frag.glsl";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniforms(const GuiTexture& gui) const;
};
