#pragma once

#include "../models/Mesh.h"
#include "../shaders/WaterShader.h"

class WaterFrameBuffers;
class Environment;

class WaterRenderer {
private:
	Mesh quad_;
	WaterShader shader_;

	int reflection_id_, refraction_id_;

public:
	WaterRenderer(const WaterFrameBuffers& fbos);

	void render(const Environment& environment);
	void prepare(const Camera& camera);
	void unbind();
};
