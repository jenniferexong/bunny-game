#pragma once

#include <memory>
#include "ImageRenderer.h"

class Shader;
class HorizontalBlurShader;
class VerticalBlurShader;
class ContrastShader;

class ImageProcessor {
protected:
	std::weak_ptr<Shader> shader_;
	std::unique_ptr<ImageRenderer> renderer_ = nullptr;

public:
	ImageProcessor() = default;
	virtual ~ImageProcessor() = default;
	
	virtual void resize(int width, int height) = 0;
	void render(int texture);
	int getOutputTexture() const;
};

class Contrast: public ImageProcessor {
private:
	std::shared_ptr<ContrastShader> c_shader_ = nullptr;
public:
	Contrast();
	void resize(int width, int height) override {}
};

class HorizontalBlur: public ImageProcessor {
private:
	std::shared_ptr<HorizontalBlurShader> h_shader_ = nullptr;
public:
	HorizontalBlur() = default;
	HorizontalBlur(int target_width, int target_height);
	void resize(int width, int height) override;
};

class VerticalBlur: public ImageProcessor {
	std::shared_ptr<VerticalBlurShader> v_shader_ = nullptr;
public:
	VerticalBlur() = default;
	VerticalBlur(int target_width, int target_height);
	void resize(int width, int height) override;
};
