#pragma once

#include "Shader.h"

class TextShader final: public Shader {
public:
	enum class UniformVariable {
		Translation, Color, CharacterWidth, EdgeTransition
	};

	static const std::string vertex_file;
	static const std::string fragment_file;

	std::unordered_map<UniformVariable, int> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniforms(const GuiText& text);
};
