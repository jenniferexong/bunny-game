#pragma once

#include "../models/Mesh.h"
#include "../shaders/WaterShader.h"

class Environment;

class WaterRenderer {
private:
	Mesh quad_;
	WaterShader shader_;

public:
	WaterRenderer();

	void render(const Environment& environment);
	void prepare(const Camera& camera);
	void unbind();
};
