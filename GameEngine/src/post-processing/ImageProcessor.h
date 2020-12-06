#pragma once

#include <memory>

#include "ImageRenderer.h"
#include "../shaders/Shader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"

class ImageProcessor {
protected:
	std::unique_ptr<Shader> shader_ = nullptr;
	std::unique_ptr<ImageRenderer> renderer_ = nullptr;

public:
	virtual void resize(int width, int height) = 0;
	void render(int texture);
	int getOutputTexture() const { return renderer_->getOutputColorTexture(); }
};

class Contrast: public ImageProcessor {
public:
	Contrast();
	void resize(int width, int height) override {}
};

class HorizontalBlur: public ImageProcessor {
private:
	HorizontalBlurShader h_shader_;
public:
	HorizontalBlur() = default;
	HorizontalBlur(int target_width, int target_height);
	void resize(int width, int height) override;
};

class VerticalBlur: public ImageProcessor {
	VerticalBlurShader v_shader_;
public:
	VerticalBlur() = default;
	VerticalBlur(int target_width, int target_height);
	void resize(int width, int height) override;
};
