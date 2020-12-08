#pragma once
#include <vector>

#include "Fbo.h"
#include "ImageProcessor.h"
#include "../models/Mesh.h"

class PostProcessor {
private:
	static Mesh quad;
	static const std::vector<float> positions;

	Fbo multisample_fbo_= Fbo(1280, 720);
	Fbo output_fbo_ = Fbo(1280, 720, DepthBufferAttachment::DepthTexture);

	Contrast contrast_;
	HorizontalBlur horizontal_blur_{ 1280, 720 };
	VerticalBlur vertical_blur_{ 1280, 720 };

public:
	static constexpr int blur_strength = 8; // {1, 2, 4, 8, 16}

	PostProcessor();
	~PostProcessor() = default;

	void resizeFbos(int width, int height);

	Fbo& getFbo() { return multisample_fbo_; }
	void process();
	void blur();
};
