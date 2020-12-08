﻿#include <GL/glew.h>

#include "PostProcessor.h"

#include "../Location.h"
#include "../Application.h"
#include "../Helper.h"
#include "../Loader.h"

const std::vector<float> PostProcessor::positions = { -1, 1, -1, -1, 1, 1, 1, -1 };
Mesh PostProcessor::quad = Mesh();

PostProcessor::PostProcessor()
{
	Print::s("POSTPROCESSOR");
	quad = app->loader->loadToVao(positions, 2);
}

void PostProcessor::process()
{
	multisample_fbo_.unbind();
	multisample_fbo_.resolveToScreen();
}

void PostProcessor::blur()
{
	multisample_fbo_.unbind();
	multisample_fbo_.resolveToFbo(output_fbo_);
	glBindVertexArray(quad.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);
	glDisable(GL_DEPTH_TEST);

	horizontal_blur_.render(output_fbo_.getColorTexture());
	vertical_blur_.render(horizontal_blur_.getOutputTexture());
	contrast_.render(vertical_blur_.getOutputTexture());

	glEnable(GL_DEPTH_TEST);	
	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);
}

void PostProcessor::resizeFbos(int width, int height)
{
	multisample_fbo_.resize(width, height);
	output_fbo_.resize(width, height);
	contrast_.resize(width, height);
	horizontal_blur_.resize(width, height);
	vertical_blur_.resize(width, height);
}



