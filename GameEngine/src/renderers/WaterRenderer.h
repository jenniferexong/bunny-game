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
	int dudv_id_;

	const float wave_speed = 0.05f;
	float move_factor_ = 0;

public:
	WaterRenderer(const WaterFrameBuffers& fbos);

	void render(const Environment& environment);
	void prepare(const Camera& camera);
	void unbind();
};
