#include <GL/glew.h>

#include "ShadowFrameBuffer.h"

#include <iostream>

#include "Application.h"
#include "Utility.h"
#include "renderers/MasterRenderer.h"

ShadowFrameBuffer::ShadowFrameBuffer(int width, int height) : width_(width), height_(height)
{
	fbo_ = createFrameBuffer();
	shadow_map_ = createDepthBufferAttachment(width_, height_);
	unbind();
}

ShadowFrameBuffer::~ShadowFrameBuffer()
{
	GLuint fbo = fbo_;
	GLuint shadow_map = shadow_map_;
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &shadow_map);
}

void ShadowFrameBuffer::bind()
{
	bindFrameBuffer(fbo_, width_, height_);
}

void ShadowFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, engine->window_width, engine->window_height);
}

void ShadowFrameBuffer::bindFrameBuffer(int fbo, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
}

int ShadowFrameBuffer::createFrameBuffer()
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	return fbo;
}

int ShadowFrameBuffer::createDepthBufferAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	Log::texture("shadow map", texture);
	return texture;
}
