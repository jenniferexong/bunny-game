#include <GL/glew.h>

#include "PostProcessor.h"

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

	multisample_fbo_ = make_shared<Fbo>(width, height, ValueType::MultiTarget);
	antialias_fbo_ = make_shared<Fbo>(
		width, height, DepthBufferAttachment::DepthTexture);
	glow_output_ = make_shared<Fbo>(
		width, height, DepthBufferAttachment::DepthTexture);

	int blur_width = width / PostProcessor::blur_strength;
	int blur_height = height / PostProcessor::blur_strength;

	blur_output_ = make_shared<Fbo>(
		width, height, DepthBufferAttachment::None);
	blur_fbo_h_ = make_shared<Fbo>(
		blur_width, blur_height, DepthBufferAttachment::None);
	blur_fbo_v_ = make_shared<Fbo>(
		blur_width, blur_height, DepthBufferAttachment::None);

	horizontal_blur_ = make_shared<HorizontalBlur>();
	vertical_blur_ = make_shared<VerticalBlur>();
	contrast_ = make_shared<Contrast>();
	//bright_ = make_shared<BrightFilter>();
	combine_ = make_shared<CombineFilter>(
		antialias_fbo_->getColorTexture(),
		blur_fbo_v_->getColorTexture()
	);

	Log::init("PostProcessor", true);
}

void PostProcessor::bloomEffect() 
{
	multisample_fbo_->unbind();
	multisample_fbo_->resolveToFbo(0, *antialias_fbo_);
	multisample_fbo_->resolveToFbo(1, *glow_output_);

	prepare();
	process(
		{ horizontal_blur_, vertical_blur_, combine_ },
		{ glow_output_, blur_fbo_h_, blur_fbo_v_, empty_fbo_ }
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
	multisample_fbo_->resolveToFbo(0, *antialias_fbo_);

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
	multisample_fbo_->resolveToFbo(0, *antialias_fbo_);
	multisample_fbo_->resolveToFbo(1, *glow_output_);

	prepare();
	process(
		{ 
			horizontal_blur_, vertical_blur_, combine_, 
			horizontal_blur_, vertical_blur_, contrast_ 
		},
		{ 
			glow_output_, 
			blur_fbo_h_, blur_fbo_v_, blur_output_, 
			blur_fbo_h_, blur_fbo_v_, blur_output_ 
		}
	);
	finish();
}

void PostProcessor::resizeFbos(int width, int height)
{
	multisample_fbo_->resize(width, height);
	antialias_fbo_->resize(width, height);
	glow_output_->resize(width, height);
	int blur_width = width / PostProcessor::blur_strength;
	int blur_height = height / PostProcessor::blur_strength;
	blur_fbo_h_->resize(blur_width, blur_height);
	blur_fbo_v_->resize(blur_width, blur_height);
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


