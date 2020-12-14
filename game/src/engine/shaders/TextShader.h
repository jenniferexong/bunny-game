#pragma once

#include <unordered_map>

#include "Shader.h"

class GuiText;

class TextShader final: public Shader {
private:
	enum class UniformVariable {
		Translation, Color, CharacterWidth, BorderColor, 
		EdgeTransition, BorderWidth, BorderEdgeTransition
	};

	static constexpr char name[] = "TextShader";
	static constexpr char vertex_file[] = "text-vert";
	static constexpr char fragment_file[] = "text-frag";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniforms(const GuiText& text);
};
