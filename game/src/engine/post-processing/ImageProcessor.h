#pragma once

#include <memory>

class Shader;
class HorizontalBlurShader;
class VerticalBlurShader;
class ContrastShader;
class BrightFilterShader;
class CombineShader;
class Fbo;

class ImageProcessor {
protected:
	std::weak_ptr<Shader> shader;

public:
	ImageProcessor() = default;
	virtual ~ImageProcessor() = default;
	
	virtual void renderToFbo(int texture, std::weak_ptr<Fbo> fbo);

	void prepare(int texture);
	void finish();
};

class BrightFilter: public ImageProcessor {
private:
	std::shared_ptr<BrightFilterShader> shader_ = nullptr;

public:
	BrightFilter();
};

class Contrast: public ImageProcessor {
private:
	std::shared_ptr<ContrastShader> shader_ = nullptr;

public:
	Contrast();
};

class CombineFilter: public ImageProcessor {
private:
	std::shared_ptr<CombineShader> shader_ = nullptr;
	int color_texture_ = -1;
	int highlight_texture_ = -1;

public:
	CombineFilter();
	CombineFilter(int color_texture, int highlight_texture);
	void renderToFbo(int texture, std::weak_ptr<Fbo> fbo) override;
};

class HorizontalBlur: public ImageProcessor {
private:
	std::shared_ptr<HorizontalBlurShader> shader_ = nullptr;

public:
	HorizontalBlur();
	void renderToFbo(int texture, std::weak_ptr<Fbo> fbo) override;
};

class VerticalBlur: public ImageProcessor {
private:
	std::shared_ptr<VerticalBlurShader> shader_ = nullptr;

public:
	VerticalBlur();
	void renderToFbo(int texture, std::weak_ptr<Fbo> fbo) override;
};
