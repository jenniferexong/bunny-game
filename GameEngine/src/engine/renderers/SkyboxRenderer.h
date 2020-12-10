#pragma once

#include <vector>

#include "../models/Mesh.h"
#include "../shaders/SkyboxShader.h"

class Skybox;
class Environment;

class SkyboxRenderer {
private:
	static const float cube_size;
	static const std::vector<float> vertex_positions;

	SkyboxShader shader_;
	Mesh cube_;

	void bindTextures(const Skybox& skybox);

public:
	SkyboxRenderer();
	~SkyboxRenderer() = default;
	void render(const Environment& environment);
};
