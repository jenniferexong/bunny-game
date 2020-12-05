#pragma once
#include <vector>

#include "Fbo.h"
#include "ImageProcessor.h"
#include "ImageRenderer.h"
#include "ContrastShader.h"
#include "../models/Mesh.h"

class PostProcessor {
private:
	static const std::vector<float> positions;
	static Mesh quad;
	Fbo fbo_ = Fbo(1280, 720, DepthBufferAttachment::DepthBuffer);

	ImageProcessor contrast_ = ImageProcessor(std::make_shared<ContrastShader>(), ImageRenderer());

public:
	PostProcessor();

	Fbo& getFbo() { return fbo_; }
	void process();
};
