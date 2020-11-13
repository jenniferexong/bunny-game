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
	static const std::vector<std::string> sky_texture_names;
	static const std::vector<std::string> star_texture_names;

	Mesh cube_;
	int day_texture_id_;
	int night_texture_id_;
	float time = 0;
	SkyboxShader shader_;

	void bindTextures(const shared_ptr<Scene>& scene);

public:
	SkyboxRenderer();
	void render(const shared_ptr<Scene>& scene);
};
