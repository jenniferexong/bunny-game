#include <GL/glew.h>

#include "PostProcessor.h"

#include <memory>

#include "ImageProcessor.h"
#include "../Application.h"
#include "../Utility.h"
#include "../Location.h"

using namespace std;

const vector<float> PostProcessor::positions = { -1, 1, -1, -1, 1, 1, 1, -1 };
Mesh PostProcessor::quad = Mesh();

PostProcessor::PostProcessor()
{
	Log::init("PostProcessor", false);

	quad = engine->loader->loadToVao(positions, 2);

	int width = engine->window_width;
	int height = engine->window_height;

	multisample_fbo_ = make_shared<Fbo>(width, height);
	antialias_fbo_ = make_shared<Fbo>(
		width, height, DepthBufferAttachment::DepthTexture);

	int blur_width = width / PostProcessor::blur_strength;
	int blur_height = height / PostProcessor::blur_strength;

	blur_output_ = make_shared<Fbo>(
		width, height, DepthBufferAttachment::None);
	blur_fbo_h_ = make_shared<Fbo>(
		blur_width, blur_height, DepthBufferAttachment::None);
	blur_fbo_v_ = make_shared<Fbo>(
		blur_width, blur_height, DepthBufferAttachment::None);

	bright_fbo_ = make_shared<Fbo>(
		width/2, height/2, DepthBufferAttachment::None);

	horizontal_blur_ = make_shared<HorizontalBlur>();
	vertical_blur_ = make_shared<VerticalBlur>();
	contrast_ = make_shared<Contrast>();
	bright_ = make_shared<BrightFilter>();
	combine_ = make_shared<CombineFilter>(
		antialias_fbo_->getColorTexture(),
		blur_fbo_v_->getColorTexture()
	);

	Log::init("PostProcessor", true);
}

void PostProcessor::bloomEffect() 
{
	multisample_fbo_->unbind();
	multisample_fbo_->resolveToFbo(*antialias_fbo_);

	prepare();
	process(
		{ bright_, horizontal_blur_, vertical_blur_, combine_ },
		{ antialias_fbo_, bright_fbo_, blur_fbo_h_, blur_fbo_v_, empty_fbo_ }
	);
	finish();
}

void PostProcessor::antiAliasToScreen()
{
	multisample_fbo_->unbind();
	multisample_fbo_->resolveToScreen();
}

void PostProcessor::process(
	const processor_pipeline& processors,
	const fbo_pipeline& fbos)
{
	for (unsigned int i = 0; i < processors.size(); i++)
		processors.at(i).lock()->renderToFbo(
			fbos.at(i).lock()->getColorTexture(),
			fbos.at(i + 1).lock()
		);
}

void PostProcessor::blur()
{
	multisample_fbo_->unbind();
	multisample_fbo_->resolveToFbo(*antialias_fbo_);

	prepare();
	process(
		{ horizontal_blur_, vertical_blur_, contrast_ },
		{ antialias_fbo_, blur_fbo_h_, blur_fbo_v_, empty_fbo_ }
	);
	finish();
}

void PostProcessor::blurToFbo()
{
	multisample_fbo_->unbind();
	multisample_fbo_->resolveToFbo(*antialias_fbo_);

	prepare();
	process(
		{ horizontal_blur_, vertical_blur_, contrast_ },
		{ antialias_fbo_, blur_fbo_h_, blur_fbo_v_, blur_output_ }
	);
	finish();
}

void PostProcessor::resizeFbos(int width, int height)
{
	multisample_fbo_->resize(width, height);
	antialias_fbo_->resize(width, height);
	int blur_width = width / PostProcessor::blur_strength;
	int blur_height = height / PostProcessor::blur_strength;
	blur_fbo_h_->resize(blur_width, blur_height);
	blur_fbo_v_->resize(blur_width, blur_height);
	bright_fbo_->resize(width/2, height/2);
}

void PostProcessor::prepare()
{
	glBindVertexArray(quad.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);
	MasterRenderer::disableDepthTest();
}

void PostProcessor::finish()
{
	MasterRenderer::enableDepthTest();
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);
	Error::gl_check(name_);
}


