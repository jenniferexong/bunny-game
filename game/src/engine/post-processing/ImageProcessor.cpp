#include <GL/glew.h>

#include "ImageProcessor.h"

#include "../Application.h"
#include "PostProcessor.h"
#include "ContrastShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"

void Contrast::renderToFbo(int texture, std::weak_ptr<Fbo> fbo)
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
			(float)engine->window_width / PostProcessor::blur_strength
		);
	}

	h_shader_->loadUniforms(target_width);

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
			(float)engine->window_height / PostProcessor::blur_strength
		);
	}

	v_shader_->loadUniforms(target_height);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (!fbo.expired())
		fbo.lock()->unbind();

	finish();
}

void ImageProcessor::prepare(int texture)
{
	shader_.lock()->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void ImageProcessor::finish()
{
	shader_.lock()->stop();
}

Contrast::Contrast() : ImageProcessor()
{
	Log::init("ImageProcessor: Contrast", false);


	c_shader_ = std::make_shared<ContrastShader>();
	c_shader_->setUp();
	shader_ = c_shader_;

	Log::init("ImageProcessor: Contrast", true);

}

HorizontalBlur::HorizontalBlur()
{
	Log::init("ImageProcessor: HorizontalBlur", false);

	h_shader_ = std::make_shared<HorizontalBlurShader>();
	h_shader_->setUp();
	shader_ = h_shader_;

	Log::init("ImageProcessor: HorizontalBlur", true);
}

VerticalBlur::VerticalBlur()
{
	Log::init("ImageProcessor: VerticalBlur", false);

	v_shader_ = std::make_shared<VerticalBlurShader>();
	v_shader_->setUp();
	shader_ = v_shader_;

	Log::init("ImageProcessor: VerticalBlur", true);
}



