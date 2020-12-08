#pragma once

#include <unordered_map>

#include "Shader.h"
#include "../gui/GuiTexture.h"

class GuiShader final : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix
	};

	static constexpr char vertex_file[] = "gui-vert";
	static constexpr char fragment_file[] = "gui-frag";

	std::unordered_map<UniformVariable, int> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadModelMatrix(const GuiTexture& gui) const;
};
