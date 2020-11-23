#pragma once

#include "../models/Mesh.h"
#include "../shaders/WaterShader.h"

class WaterFrameBuffers;
class Environment;

class WaterRenderer {
private:
	Mesh quad_;
	WaterShader shader_;

	int reflection_id_, refraction_id_, depth_id_;
	int dudv_id_, normal_id_;

	static const float wave_speed;
	float move_factor_ = 0;

public:
	WaterRenderer() = default;
	WaterRenderer(const WaterFrameBuffers& fbos);

	void render(const Environment& environment);
	void prepare(const Environment& environment);
	void unbind();
};
