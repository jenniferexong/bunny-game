#pragma once

#include <unordered_map>

#include "Shader.h"

class ShadowShader final: public Shader {
private:
	enum class UniformVariable {
		Matrix
	};

	static constexpr char name[] = "ShadowShader";
	static constexpr char vertex_file[] = "shadow-vert";
	static constexpr char fragment_file[] = "shadow-frag";

	std::unordered_map<UniformVariable, int> locations_;

public:
	void setUp() override;
	void bindAttributes() override;
	void getAllUniformLocations() override;
	void loadUniforms(glm::mat4 matrix);
};
