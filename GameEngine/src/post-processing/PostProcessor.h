#pragma once
#include <vector>

#include "Fbo.h"
#include "ImageProcessor.h"
#include "../models/Mesh.h"

class PostProcessor {
private:
	static const std::vector<float> positions;
	static Mesh quad;
	Fbo multisample_fbo_= Fbo(1280, 720);
	Fbo output_fbo_ = Fbo(1280, 720, DepthBufferAttachment::DepthTexture);

	Contrast contrast_ = Contrast();
	HorizontalBlur horizontal_blur_ = HorizontalBlur(1280, 720);
	VerticalBlur vertical_blur_ = VerticalBlur(1280, 720);

public:
	static const int blur_strength;

	PostProcessor();

	void resizeFbos(int width, int height);

	Fbo& getFbo() { return multisample_fbo_; }
	void process();
	void blur();
};
