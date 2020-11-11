﻿#pragma once

#include <string>
#include <vector>

#include "../models/Mesh.h"

#include "../shaders/SkyboxShader.h"


class SkyboxRenderer {
private:
	static const float cube_size;
	static const std::vector<float> vertex_positions;
	static const std::vector<std::string> sky_texture_names;
	static const std::vector<std::string> star_texture_names;

	Mesh cube_;
	int sky_texture_id_;
	int star_texture_id_;
	SkyboxShader shader_;

	void bindTextures();

public:
	SkyboxRenderer();
	void render();
};
