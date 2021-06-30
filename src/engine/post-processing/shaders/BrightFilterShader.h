#pragma once

#include <unordered_map>

#include "../../shaders/Shader.h"

class BrightFilterShader final : public Shader {

	enum class UniformVariable { };

	static constexpr char name[] = "BrightFilterShader";
	static constexpr char vertex_file[] = "post-processing/default-vert.glsl";
	static constexpr char fragment_file[] = "post-processing/bright-filter-frag.glsl";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override {}

public:
	void setUp() override;
};
