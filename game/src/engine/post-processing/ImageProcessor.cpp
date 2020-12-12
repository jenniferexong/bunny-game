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
	if (!fbo.expired()) {
		fbo.lock()->bind();
		Print::s("rendering to blur output fbo");
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // quad

	if (!fbo.expired())
		fbo.lock()->unbind();

	finish();
}

void HorizontalBlur::renderToFbo(int texture, std::weak_ptr<Fbo> fbo)
{
	prepare(texture);

	float target_width;
	if (!fbo.expired()) {
		fbo.lock()->bind();
		target_width = (float)fbo.lock()->getWidth();
	} else { // rendering to screen
		target_width = (float)engine->window_width / PostProcessor::blur_strength;
	}

	h_shader_->loadUniforms(target_width);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // single quad

	if (!fbo.expired())
		fbo.lock()->unbind();

	finish();
}

void VerticalBlur::renderToFbo(int texture, std::weak_ptr<Fbo> fbo)
{
	prepare(texture);

	float target_height;
	if (!fbo.expired()) {
		fbo.lock()->bind();
		target_height = (float)fbo.lock()->getHeight();
	} else { // rendering to screen
		target_height = (float)engine->window_height / PostProcessor::blur_strength;
	}

	v_shader_->loadUniforms(target_height);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // single quad

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
	c_shader_ = std::make_shared<ContrastShader>();
	c_shader_->setUp();
	shader_ = c_shader_;
}

HorizontalBlur::HorizontalBlur()
{
	h_shader_ = std::make_shared<HorizontalBlurShader>();
	h_shader_->setUp();
	shader_ = h_shader_;
}

VerticalBlur::VerticalBlur()
{
	v_shader_ = std::make_shared<VerticalBlurShader>();
	v_shader_->setUp();
	shader_ = v_shader_;
}



