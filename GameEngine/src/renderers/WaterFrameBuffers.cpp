#include <gl/glew.h>

#include "WaterFrameBuffers.h"

#include <vector>

#include "../Application.h"

int WaterFrameBuffers::reflection_width = 1280;
int WaterFrameBuffers::reflection_height = 720;
int WaterFrameBuffers::refraction_width = 1280;
int WaterFrameBuffers::refraction_height = 720;

WaterFrameBuffers::WaterFrameBuffers()
{
	// initialize frame buffers
	// reflection
	reflection_frame_buffer_ = createFrameBuffer();	
	reflection_texture_ = createTextureAttachment(reflection_width, reflection_height);
	reflection_depth_buffer_ = createDepthBufferAttachment(reflection_width, reflection_height);
	unbindCurrentFrameBuffer();

	// refraction
	refraction_frame_buffer_ = createFrameBuffer();
	refraction_texture_ = createTextureAttachment(refraction_width, refraction_height);
	refraction_depth_texture_ = createDepthTextureAttachment(refraction_width, refraction_height);
	unbindCurrentFrameBuffer();
}

WaterFrameBuffers::~WaterFrameBuffers()
{
	std::vector<int> frame_buffers = { reflection_frame_buffer_, refraction_frame_buffer_ };
	std::vector<int> render_buffers = { reflection_depth_buffer_};
	std::vector<int> textures = { reflection_texture_, refraction_texture_, refraction_depth_texture_ };

	glDeleteFramebuffers(frame_buffers.size(), (GLuint*)frame_buffers.data());
	glDeleteRenderbuffers(render_buffers.size(), (GLuint*) render_buffers.data());
	glDeleteTextures(textures.size(), (GLuint*) textures.data());
}

void WaterFrameBuffers::bindReflectionFrameBuffer()
{
	bindFrameBuffer(reflection_frame_buffer_, reflection_width, reflection_height);
}

void WaterFrameBuffers::bindRefractionFrameBuffer()
{
	bindFrameBuffer(refraction_frame_buffer_, refraction_width, refraction_height);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, MasterRenderer::window_width, MasterRenderer::window_height);
}

void WaterFrameBuffers::bindFrameBuffer(int frame_buffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0); // ensure texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	glViewport(0, 0, width, height);
}

int WaterFrameBuffers::createFrameBuffer()
{
	GLuint frame_buffer;
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0); // always 0 location
	return frame_buffer;
}

int WaterFrameBuffers::createTextureAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}

int WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}

int WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
{
	GLuint depth_buffer;
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	return depth_buffer;
}
