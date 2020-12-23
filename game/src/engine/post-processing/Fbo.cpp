#include <GL/glew.h>

#include "Fbo.h"

#include "../Utility.h"
#include "../Application.h"
#include "../renderers/MasterRenderer.h"

Fbo::Fbo(int width, int height, Type type) : width_(width), height_(height)
{
	type_ = DepthBufferAttachment::DepthBuffer;
	multi_sample_ = true;
	if (type == Type::MultiTarget)
		multi_target_ = true;
	initialise();
}

Fbo::Fbo(
	int width,
	int height,
	DepthBufferAttachment type,
	bool clamp_to_edge
): 
	type_(type),
	clamp_to_edge_(clamp_to_edge),
	width_(width),
	height_(height)
{
	initialise();
}

Fbo::Fbo(int width, int height, DepthBufferAttachment type): 
	type_(type), width_(width), height_(height)
{
	initialise();
}

void Fbo::initialise()
{
	createFrameBuffer();
	string attachment = "";
	if (multi_sample_) {
		attachment.append("color buffer loc-0 (ms)");
		color_buffer_1_ = createColorBufferAttachment(0);
	} 
	else {
		attachment.append("color texture");
		createTextureAttachment();
	}
	// multi-target fbo
	if (multi_target_) {
		attachment.append(", color buffer loc-1 (ms)");
		color_buffer_2_ = createColorBufferAttachment(1);
	}
	if (type_ == DepthBufferAttachment::DepthTexture) {
		attachment.append(", depth texture");
		createDepthTextureAttachment();
	}
	else if (type_ == DepthBufferAttachment::DepthBuffer) {
		attachment.append(", depth buffer");
		createDepthBufferAttachment();
	}
	unbind();
	Log::fbo(attachment, fbo_id_);
	Error::gl_check(name);
}

/**
 * Copies fbo data into another fbo so it can be sampled from,
 * applies anti-aliasing
 * @param read_attachment color attachment to be read from
 */
void Fbo::resolveToFbo(int read_attachment, Fbo& output)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output.fbo_id_);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id_);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + read_attachment);
	glBlitFramebuffer(
		0, 0, width_, height_,
		0, 0, output.width_, output.height_,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	unbind();
}

void Fbo::resolveToScreen()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id_);
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(
		0, 0, width_, height_,
		0, 0, engine->window_width, engine->window_height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
	unbind();
}

void Fbo::resize(int width, int height)
{
	if (type_ == DepthBufferAttachment::Uninitialised)
		Error::exit("tried to resize uninitialized fbo");

	cleanUp();
	width_ = width;
	height_ = height;
	initialise();
}

int Fbo::getColorTexture() const
{
	if (color_texture_ == -1)
		Error::exit(
			"fbo id: " 
			+ std::to_string(fbo_id_) 
			+ ", color texture not initialized"
		);

	return color_texture_;
}

int Fbo::getDepthTexture() const
{
	if (depth_texture_ == -1)
		Error::exit(
			"fbo id: " 
			+ std::to_string(fbo_id_) 
			+ ", depth texture not initialized"
		);

	return depth_texture_;
}

void Fbo::cleanUp()
{
	glDeleteFramebuffers(1, (GLuint*)&fbo_id_);
	glDeleteTextures(1, (GLuint*)&color_texture_);

	if (depth_texture_ > -1)
		glDeleteTextures(1, (GLuint*)&depth_texture_);
	if (depth_buffer_ > -1)
		glDeleteRenderbuffers(1, (GLuint*)&depth_buffer_);
	if (color_buffer_1_ > -1) 
		glDeleteRenderbuffers(1, (GLuint*)&color_buffer_1_);
	if (color_buffer_2_ > -1) 
		glDeleteRenderbuffers(1, (GLuint*)&color_buffer_2_);

	fbo_id_ = -1;
	color_texture_ = -1;
	depth_texture_ = -1;
	depth_buffer_ = -1;
	color_buffer_1_ = -1;
	color_buffer_2_ = -1;
}

Fbo::~Fbo()
{
	Log::destroy("fbo", fbo_id_);
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
	glViewport(0, 0, engine->window_width, engine->window_height);
}

void Fbo::createFrameBuffer()
{
	glGenFramebuffers(1, (GLuint*)&fbo_id_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
	determineDrawBuffers();
}

void Fbo::createTextureAttachment()
{
	glGenTextures(1, (GLuint*)&color_texture_);
	glBindTexture(GL_TEXTURE_2D, color_texture_);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (clamp_to_edge_) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture_, 0);

	Log::texture("color texture attachment", color_texture_);
}

void Fbo::createDepthTextureAttachment()
{
	glGenTextures(1, (GLuint*)&depth_texture_);
	glBindTexture(GL_TEXTURE_2D, depth_texture_);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width_, height_,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture_, 0);

	Log::texture("depth texture attachment", depth_texture_);
}

/* Mulisampled */
int Fbo::createColorBufferAttachment(int attachment)
{
	GLuint buffer;
	//glGenRenderbuffers(1, (GLuint*)&color_buffer_);
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	glRenderbufferStorageMultisample(
		GL_RENDERBUFFER, 4, GL_RGBA8, width_, height_);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0 + attachment,
		GL_RENDERBUFFER,
		buffer
	);

	Log::s("created color buffer attachment: " + std::to_string(buffer));
	return buffer;
}

void Fbo::determineDrawBuffers() 
{
	vector<GLenum> buffers = { GL_COLOR_ATTACHMENT0 };
	if (multi_target_)
		buffers.push_back(GL_COLOR_ATTACHMENT1);
	glDrawBuffers(buffers.size(), buffers.data());
}

void Fbo::createDepthBufferAttachment()
{
	glGenRenderbuffers(1, (GLuint*)&depth_buffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_);
	if (multi_sample_)
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, width_, height_
		);
	else
		glRenderbufferStorage(
			GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_
		);

	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_
	);

	Log::s("created depth buffer attachment: " + std::to_string(depth_buffer_));
}



