#pragma once
#include <vector>

#include "Fbo.h"
#include "ImageProcessor.h"
#include "../models/Mesh.h"

using processor_pipeline = std::vector<std::weak_ptr<ImageProcessor>>;
using fbo_pipeline = std::vector<std::weak_ptr<Fbo>>;

class PostProcessor {
private:
	static constexpr char name_[] = "PostProcessor";
	static Mesh quad;
	static const std::vector<float> positions;

	std::weak_ptr<Fbo> empty_fbo_;

	std::shared_ptr<Fbo> multisample_fbo_;
	std::shared_ptr<Fbo> antialias_fbo_;

	std::shared_ptr<Fbo> blur_fbo_h_;
	std::shared_ptr<Fbo> blur_fbo_v_;

	// used for background of pause screen (don't resize on window resize!)
	std::shared_ptr<Fbo> blur_output_;

	// image processors
	std::shared_ptr<HorizontalBlur> horizontal_blur_;
	std::shared_ptr<VerticalBlur> vertical_blur_;
	std::shared_ptr<Contrast> contrast_;

	void process(const processor_pipeline& processors, const fbo_pipeline& fbos);
	void prepare();
	void finish();

public:
	static constexpr int blur_strength = 8; // {1, 2, 4, 8, 16}

	PostProcessor();
	~PostProcessor() = default;

	void startProcessing() { multisample_fbo_->bind(); }
	void antiAliasToScreen();
	void blur();
	void blurToFbo();

	void resizeFbos(int width, int height);

	int getBlurTexture() const { return blur_output_->getColorTexture(); }
};
