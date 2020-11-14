#pragma once

#include <string>
#include <vector>

#include "../models/Mesh.h"
#include "../scene/Scene.h"
#include "../shaders/SkyboxShader.h"

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
	void render(const Environment& environment, bool progress_time);
};
