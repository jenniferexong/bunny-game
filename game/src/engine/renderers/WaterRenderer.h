#pragma once

#include <memory>

#include "../models/Mesh.h"
#include "../post-processing/Fbo.h"
#include "../shaders/WaterShader.h"

class WaterFrameBuffers;
class Environment;

class WaterRenderer {
private:
	Mesh quad_;
	WaterShader shader_;

	std::unique_ptr<Fbo> reflection_fbo_;
	std::unique_ptr<Fbo> refraction_fbo_;

	int dudv_id_, normal_id_;

public:
	WaterRenderer();
	~WaterRenderer() = default;
	Fbo& getReflectionFbo() { return *reflection_fbo_; }
	Fbo& getRefractionFbo() { return *refraction_fbo_; }

	void render(const Environment& environment);
	void prepare(const Environment& environment);
	void unbind();
};
