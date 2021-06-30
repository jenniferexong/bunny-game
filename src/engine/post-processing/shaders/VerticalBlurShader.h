#pragma once

#include <unordered_map>

#include "../../shaders/Shader.h"

class VerticalBlurShader final : public Shader {

	enum class UniformVariable {
		TargetHeight
	};

	static constexpr char name[] = "VerticalBlurShader";

	static constexpr char vertex_file[] = "post-processing/vertical-blur-vert.glsl";
	static constexpr char fragment_file[] = "post-processing/blur-frag.glsl";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniforms(float target_height);
};
