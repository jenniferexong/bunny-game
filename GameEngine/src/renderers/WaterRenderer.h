#pragma once

#include "../models/Mesh.h"
#include "../post-processing/Fbo.h"
#include "../shaders/WaterShader.h"

class WaterFrameBuffers;
class Environment;

class WaterRenderer {
private:
	Mesh quad_;
	WaterShader shader_;

	Fbo reflection_fbo_ = Fbo(1280, 720, DepthBufferAttachment::DepthBuffer, false);
	Fbo refraction_fbo_ = Fbo(1280, 720, DepthBufferAttachment::DepthTexture, false);

	int dudv_id_, normal_id_;

	static const float wave_speed;
	float move_factor_ = 0;

public:
	WaterRenderer();
	~WaterRenderer() = default;
	Fbo& getReflectionFbo() { return reflection_fbo_; }
	Fbo& getRefractionFbo() { return refraction_fbo_; }

	void render(const Environment& environment);
	void prepare(const Environment& environment);
	void unbind();
};
