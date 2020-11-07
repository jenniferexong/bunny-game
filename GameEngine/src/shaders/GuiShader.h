#pragma once

#include <unordered_map>

#include "Shader.h"
#include "../gui/GuiTexture.h"

class GuiShader final : public Shader {

	enum class UniformVariable {
		TransformationMatrix
	};

	static const std::string vertex_file;
	static const std::string fragment_file;

	std::unordered_map<UniformVariable, int> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadModelMatrix(const GuiTexture& gui) const;
};
