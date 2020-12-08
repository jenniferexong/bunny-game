#include <GL/glew.h>

#include "ImageProcessor.h"

#include "ContrastShader.h"
#include "HorizontalBlurShader.h"
#include "PostProcessor.h"
#include "VerticalBlurShader.h"

#include "ImageRenderer.h"

void ImageProcessor::render(int texture)
{
	shader_.lock()->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	renderer_->renderQuad();
	shader_.lock()->stop();
}
int ImageProcessor::getOutputTexture() const
{
	return renderer_->getOutputColorTexture();
}

Contrast::Contrast() : ImageProcessor()
{
	renderer_ = std::make_unique<ImageRenderer>();
	c_shader_ = std::make_shared<ContrastShader>();
	c_shader_->setUp();
	shader_ = c_shader_;
}

HorizontalBlur::HorizontalBlur(int target_width, int target_height)
{
	h_shader_ = std::make_shared<HorizontalBlurShader>();
	float width = (float)target_width / PostProcessor::blur_strength;
	float height = (float)target_height / PostProcessor::blur_strength;
	h_shader_->setUp();
	h_shader_->start();
	h_shader_->loadUniforms(width);
	h_shader_->stop();

	shader_ = h_shader_;
	renderer_ = std::make_unique<ImageRenderer>((int)width, (int)height);
}

void HorizontalBlur::resize(int width, int height)
{
	float w = (float)width / PostProcessor::blur_strength;
	float h = (float)height / PostProcessor::blur_strength;
	h_shader_->start();
	h_shader_->loadUniforms(w);
	h_shader_->stop();
	renderer_->getFbo().resize((int)w, (int)h);
}

VerticalBlur::VerticalBlur(int target_width, int target_height)
{
	v_shader_ = std::make_shared<VerticalBlurShader>();
	float width = (float) target_width / PostProcessor::blur_strength;
	float height = (float) target_height / PostProcessor::blur_strength;
	v_shader_->setUp();
	v_shader_->start();
	v_shader_->loadUniforms(height);
	v_shader_->stop();

	shader_ = v_shader_;
	renderer_ = std::make_unique<ImageRenderer>((int)width, (int)height);
}

void VerticalBlur::resize(int width, int height)
{
	float w = (float) width / PostProcessor::blur_strength;
	float h = (float) height / PostProcessor::blur_strength;
	v_shader_->start();
	v_shader_->loadUniforms(h);
	v_shader_->stop();
	renderer_->getFbo().resize((int)w, (int)h);
}




