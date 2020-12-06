#pragma once
#include "Fbo.h"

/**
 * Handles rendering texture to the screen, or another fbo
 */
class ImageRenderer {
private:
	Fbo fbo_;

public:
	/* For rendering to screen */
	ImageRenderer() = default;

	/* For rendering to another fbo */
	ImageRenderer(int width, int height) : fbo_(Fbo(width, height, DepthBufferAttachment::None)) {}

	/* Renders texture to entire screen, or to another fbo */
	void renderQuad();
	int getOutputColorTexture() const { return fbo_.getColorTexture(); }

	Fbo& getFbo() { return fbo_; }
};
