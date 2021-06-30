#pragma once

#include <unordered_map>

#include "../../shaders/Shader.h"

class CombineShader final : public Shader {

	enum class UniformVariable { ColorTexture, HighlightTexture};

	static constexpr char name[] = "ContrastShader";
	static constexpr char vertex_file[] = "post-processing/default-vert.glsl";
	static constexpr char fragment_file[] = "post-processing/combine-frag.glsl";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void connectTextureUnits();
};
