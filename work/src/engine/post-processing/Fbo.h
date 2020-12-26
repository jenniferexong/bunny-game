#pragma once

enum class DepthBufferAttachment {
	Uninitialised, None, DepthTexture, DepthBuffer
};

enum class Type {
	MultiSample, MultiTarget
};

class Fbo {
private:
	static constexpr char name[] = "Fbo";
	DepthBufferAttachment type_ = DepthBufferAttachment::Uninitialised;

	bool clamp_to_edge_ = true;
	bool multi_sample_ = false;
	bool multi_target_ = false;

	int width_ = 0;
	int height_ = 0;

	int fbo_id_ = -1;

	int color_texture_ = -1;
	int depth_texture_ = -1;

	int depth_buffer_ = -1;

	int color_buffer_1_ = -1;
	int color_buffer_2_ = -1;

	void createFrameBuffer();

	/** 
	 * Color buffer attachment for this fbo
	 */
	void createTextureAttachment();

	/**
	 * Depth buffer in the form of a texture, can be sampled from
	 */
	void createDepthTextureAttachment();

	/**
	 * Depth buffer in the form of a render buffer, can't be sampled from
	 */
	void createDepthBufferAttachment();

	/**
	 * Color buffer in the form of a render buffer,
	 * can't be sampled from, mulitsampling 
	 */
	int createColorBufferAttachment(int attachment);

	void determineDrawBuffers();

public:
	Fbo() = default;
	
	/**
	 * Fbos with multisampling and/or mutitarget - always render buffer
	 */
	Fbo(int width, int height, Type type);

	Fbo(int width, int height, DepthBufferAttachment type);
	Fbo(int width, int height, DepthBufferAttachment type, bool clamp_to_edge);

	void cleanUp();
	~Fbo();

	void initialise();

	/**
	 * Copies fbo data into another fbo so it can be sampled from,
	 * applies anti-aliasing
	 * @param read_attachment color attachment to be read from
	 */
	void resolveToFbo(int read_attachment, Fbo& output);
	void resolveToScreen();

	void resize(int width, int height);

	void bind() const;
	void bindToRead() const;

	/**
	 * Sets default frame buffer (screen) as the current render target
	 */
	void unbind() const;

	bool intialised() { return type_ != DepthBufferAttachment::Uninitialised; }
	int getColorTexture() const;
	int getDepthTexture() const;
	int getWidth() const { return width_; }
	int getHeight() const { return height_; }
};
