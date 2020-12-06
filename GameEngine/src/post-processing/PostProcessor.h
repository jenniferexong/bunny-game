#pragma once
#include <vector>

#include "Fbo.h"
#include "ImageProcessor.h"
#include "../models/Mesh.h"

class PostProcessor {
private:
	static const std::vector<float> positions;
	static Mesh quad;
	Fbo fbo_ = Fbo(1280, 720, DepthBufferAttachment::DepthBuffer);

	Contrast contrast_ = Contrast();
	HorizontalBlur horizontal_blur_ = HorizontalBlur(1280, 720);
	VerticalBlur vertical_blur_ = VerticalBlur(1280, 720);

public:
	static const int blur_strength;

	PostProcessor();

	void resizeFbos(int width, int height);

	Fbo& getFbo() { return fbo_; }
	void process();
};
