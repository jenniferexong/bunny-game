#pragma once

#include <string>
#include <vector>

#include "../models/Mesh.h"

#include "../shaders/SkyboxShader.h"


class SkyboxRenderer {
private:
	static const float cube_size;
	static const std::vector<float> vertex_positions;
	static const std::vector<std::string> texture_names;

	Mesh cube_;
	int texture_id_;
	SkyboxShader shader_;

public:
	SkyboxRenderer();
	void render();
};
