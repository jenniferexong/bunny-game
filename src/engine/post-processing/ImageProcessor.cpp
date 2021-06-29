#include <GL/glew.h>

#include "ImageProcessor.h"

#include "../Engine.h"
#include "PostProcessor.h"
#include "shaders/ContrastShader.h"
#include "shaders/HorizontalBlurShader.h"
#include "shaders/VerticalBlurShader.h"
#include "shaders/BrightFilterShader.h"
#include "shaders/CombineShader.h"

void ImageProcessor::renderToFbo(int texture, std::weak_ptr<Fbo> fbo)
{
	prepare(texture);
	if (!fbo.expired())
		fbo.lock()->bind();

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 

	if (!fbo.expired())
		fbo.lock()->unbind();

	finish();
}

void CombineFilter::renderToFbo(int texture, std::weak_ptr<Fbo> fbo)
{
	shader.lock()->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_texture_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, highlight_texture_);

	// rendering to fbo
	if (!fbo.expired())
		fbo.lock()->bind();

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (!fbo.expired())
		fbo.lock()->unbind();

	finish();
}

void HorizontalBlur::renderToFbo(int texture, std::weak_ptr<Fbo> fbo)
{
	prepare(texture);

	float target_width;
	// rendering to fbo
	if (!fbo.expired()) {
		fbo.lock()->bind();
		target_width = (float)fbo.lock()->getWidth();
	} 
	// rendering to screen
	else { 
		target_width = (
			(float)Engine::instance->window_width / PostProcessor::blur_strength);
	}

	shader_->loadUniforms(target_width);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (!fbo.expired())
		fbo.lock()->unbind();

	finish();
}

void VerticalBlur::renderToFbo(int texture, std::weak_ptr<Fbo> fbo)
{
	prepare(texture);

	float target_height;
	// rendering to fbo
	if (!fbo.expired()) {
		fbo.lock()->bind();
		target_height = (float)fbo.lock()->getHeight();
	} 
	// rendering to screen
	else { 
		target_height = (
			(float)Engine::instance->window_height / PostProcessor::blur_strength);
	}

	shader_->loadUniforms(target_height);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (!fbo.expired())
		fbo.lock()->unbind();

	finish();
}

void ImageProcessor::prepare(int texture)
{
	shader.lock()->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void ImageProcessor::finish()
{
	shader.lock()->stop();
}

CombineFilter::CombineFilter()
{
	Error::exit(
		"CombineFilter: wrong contructor called, \
		must init color_texture_ and highlight_texture_"
	);
}

CombineFilter::CombineFilter(int color_texture, int highlight_texture):
	color_texture_(color_texture), 
	highlight_texture_(highlight_texture)
{
	Log::init("ImageProcessor: CombineFilter", false);

	shader_ = std::make_shared<CombineShader>();
	shader_->setUp();
	shader = shader_;

	Log::init("ImageProcessor: CombineFilter", true);
}

BrightFilter::BrightFilter()
{
	Log::init("ImageProcessor: BrightFilter", false);

	shader_ = std::make_shared<BrightFilterShader>();
	shader_->setUp();
	shader = shader_;

	Log::init("ImageProcessor: BrightFilter", true);
}

Contrast::Contrast()
{
	Log::init("ImageProcessor: Contrast", false);

	shader_ = std::make_shared<ContrastShader>();
	shader_->setUp();
	shader = shader_;

	Log::init("ImageProcessor: Contrast", true);
}

HorizontalBlur::HorizontalBlur()
{
	Log::init("ImageProcessor: HorizontalBlur", false);

	shader_ = std::make_shared<HorizontalBlurShader>();
	shader_->setUp();
	shader = shader_;

	Log::init("ImageProcessor: HorizontalBlur", true);
}

VerticalBlur::VerticalBlur()
{
	Log::init("ImageProcessor: VerticalBlur", false);

	shader_ = std::make_shared<VerticalBlurShader>();
	shader_->setUp();
	shader = shader_;

	Log::init("ImageProcessor: VerticalBlur", true);
}
