#pragma once

class ShadowFrameBuffer {
private:
	int width_;
	int height_;

	int fbo_ = -1;
	int shadow_map_ = -1;

	static void bindFrameBuffer(int fbo, int width, int height);
	static int createFrameBuffer();
	static int createDepthBufferAttachment(int width, int height);

public:
	ShadowFrameBuffer() = default;
	ShadowFrameBuffer(int width, int height);
	~ShadowFrameBuffer();

	void bind();
	void unbind();

	int getFbo() const { return fbo_; }
	int getShadowMap() const { return shadow_map_; }
};

