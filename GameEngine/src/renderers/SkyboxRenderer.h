#pragma once

#include <vector>

#include "../models/Mesh.h"
#include "../shaders/SkyboxShader.h"

class Environment;

class SkyboxRenderer {
private:
	static const float cube_size;
	static const std::vector<float> vertex_positions;

	SkyboxShader shader_;
	Mesh cube_;

	float time = 0;

	void bindTextures(const Environment& environment, bool progress_time);

public:
	SkyboxRenderer();
	~SkyboxRenderer() = default;
	void render(const Environment& environment, bool progress_time);
};
