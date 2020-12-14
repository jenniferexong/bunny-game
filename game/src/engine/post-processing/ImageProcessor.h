#pragma once

#include <memory>

class Shader;
class HorizontalBlurShader;
class VerticalBlurShader;
class ContrastShader;
class Fbo;

class ImageProcessor {
protected:
	std::weak_ptr<Shader> shader_;

public:
	ImageProcessor() = default;
	virtual ~ImageProcessor() = default;
	
	virtual void renderToFbo(int texture, std::weak_ptr<Fbo> fbo) = 0;

	void prepare(int texture);
	void finish();
};

class Contrast: public ImageProcessor {
private:
	std::shared_ptr<ContrastShader> c_shader_ = nullptr;

public:
	Contrast();
	void renderToFbo(int texture, std::weak_ptr<Fbo> fbo) override;
};

class HorizontalBlur: public ImageProcessor {
private:
	std::shared_ptr<HorizontalBlurShader> h_shader_ = nullptr;

public:
	HorizontalBlur();
	void renderToFbo(int texture, std::weak_ptr<Fbo> fbo) override;
};

class VerticalBlur: public ImageProcessor {
private:
	std::shared_ptr<VerticalBlurShader> v_shader_ = nullptr;

public:
	VerticalBlur();
	void renderToFbo(int texture, std::weak_ptr<Fbo> fbo) override;
};
