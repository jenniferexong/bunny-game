#pragma once

#include <unordered_map>

#include "../shaders/Shader.h"

class ContrastShader final : public Shader {

	enum class UniformVariable { };

	static constexpr char name[] = "ContrastShader";
	static constexpr char vertex_file[] = "post-processing/contrast-vert";
	static constexpr char fragment_file[] = "post-processing/contrast-frag";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override {}

public:
	void setUp() override;
};
