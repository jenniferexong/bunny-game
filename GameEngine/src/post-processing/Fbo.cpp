#include <GL/glew.h>

#include "Fbo.h"

#include "../Helper.h"
#include "../renderers/MasterRenderer.h"

Fbo::Fbo(int width, int height, DepthBufferAttachment type, bool clamp_to_edge)
	: width_(width), height_(height), type_(type), clamp_to_edge_(clamp_to_edge)
{
	initialise();
}

Fbo::Fbo(int width, int height, DepthBufferAttachment type) : width_(width), height_(height), type_(type)
{
	initialise();
}

void Fbo::initialise()
{
	createFrameBuffer();
	createTextureAttachment();
	string attachment = "color texture";
	if (type_ == DepthBufferAttachment::DepthTexture) {
		attachment.append(", depth texture");
		createDepthTextureAttachment();
	}
	else if (type_ == DepthBufferAttachment::DepthBuffer) {
		attachment.append(", depth buffer");
		createDepthBufferAttachment();
	}
	unbind();
	Print::fbo(attachment, fbo_id_);
}

void Fbo::resize(int width, int height)
{
	if (type_ == DepthBufferAttachment::Uninitialised) {
		Print::s("TRYING TO RESIZE UNINITIALISED FBO");
		return;
	}
	cleanUp();
	width_ = width;
	height_ = height;
	initialise();
}

int Fbo::getColorTexture() const
{
	if (color_texture_ == (GLuint)-1)
		Print::s("Color texture not initialized");

	return color_texture_;
}

int Fbo::getDepthTexture() const
{
	if (depth_texture_ == (GLuint)-1)
		Print::s("Depth texture not initialized");

	return depth_texture_;
}

void Fbo::cleanUp()
{
	glDeleteFramebuffers(1, &fbo_id_);
	glDeleteTextures(1, &color_texture_);

	if (depth_texture_ > -1)
		glDeleteTextures(1, &depth_texture_);
	if (depth_buffer_ > -1)
		glDeleteRenderbuffers(1, &depth_buffer_);
	if (color_buffer_ > -1) 
		glDeleteRenderbuffers(1, &color_buffer_);

	fbo_id_ = -1;
	color_texture_ = -1;
	depth_texture_ = -1;
	depth_buffer_ = -1;
	color_buffer_ = -1;
}

Fbo::~Fbo()
{
	Print::s("fbo destroyed: " + std::to_string(fbo_id_));
	cleanUp();
}

void Fbo::bind() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id_);
	glViewport(0, 0, width_, height_);
}

void Fbo::bindToRead() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id_);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void Fbo::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, MasterRenderer::window_width, MasterRenderer::window_height);
}

void Fbo::createFrameBuffer()
{
	glGenFramebuffers(1, &fbo_id_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
	glDrawBuffer(GL_COLOR_ATTACHMENT0); // always 0 location
}

void Fbo::createTextureAttachment()
{
	glGenTextures(1, &color_texture_);
	glBindTexture(GL_TEXTURE_2D, color_texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (clamp_to_edge_) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture_, 0);

	Print::texture("color texture attachment", color_texture_);
}

void Fbo::createDepthTextureAttachment()
{
	glGenTextures(1, &depth_texture_);
	glBindTexture(GL_TEXTURE_2D, depth_texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture_, 0);

	Print::texture("depth texture attachment", depth_texture_);
}

void Fbo::createDepthBufferAttachment()
{
	glGenRenderbuffers(1, &depth_buffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_);

	Print::s("created depth buffer attachment: " + std::to_string(depth_buffer_));
}



