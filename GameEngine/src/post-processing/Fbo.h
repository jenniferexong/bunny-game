#pragma once

enum class DepthBufferAttachment {
	Uninitialised, None, DepthTexture, DepthBuffer
};

class Fbo {
private:
	DepthBufferAttachment type_ = DepthBufferAttachment::Uninitialised;

	bool clamp_to_edge_ = true;
	bool multi_sample_ = false;

	int width_ = 0;
	int height_ = 0;

	GLuint fbo_id_ = -1;

	GLuint color_texture_ = -1;
	GLuint depth_texture_ = -1;

	GLuint depth_buffer_ = -1;
	GLuint color_buffer_ = -1;

	void createFrameBuffer();

	/* Color buffer attachment for this fbo */
	void createTextureAttachment();
	/* Depth buffer in the form of a texture, can be sampled from */
	void createDepthTextureAttachment();
	/* Depth buffer in the form of a render buffer, can't be sampled from */
	void createDepthBufferAttachment();
	/* Color buffer in the form of a render buffer, can't be sampled from, mulitsampling */
	void createColorBufferAttachment();

public:
	Fbo() = default;
	// Fbos with multisampling - always render buffer
	Fbo(int width, int height);
	Fbo(int width, int height, DepthBufferAttachment type);
	Fbo(int width, int height, DepthBufferAttachment type, bool clamp_to_edge);

	void cleanUp();
	~Fbo();

	void initialise();

	/* Copies fbo data into another fbo so it can be sampled from, applies anti-aliasing*/
	void resolveToFbo(Fbo& output);
	void resolveToScreen();

	void resize(int width, int height);

	/* Sets fbo to be the current render target */
	void bind() const;
	void bindToRead() const;

	/* Sets default frame buffer (screen) as the current render target */
	void unbind() const;

	bool intialised() { return type_ != DepthBufferAttachment::Uninitialised; }
	int getColorTexture() const;
	int getDepthTexture() const;
};
