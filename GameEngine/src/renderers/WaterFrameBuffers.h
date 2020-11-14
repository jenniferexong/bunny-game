#pragma once

class WaterFrameBuffers {
private:
	static int reflection_width;
	static int reflection_height;

	static int refraction_width;
	static int refraction_height;

	int reflection_frame_buffer_;
	int reflection_texture_;
	int reflection_depth_buffer_;

	int refraction_frame_buffer_;
	int refraction_texture_;
	int refraction_depth_texture_;

	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();

	void initializeFrameBuffers();

	void bindFrameBuffer(int frame_buffer, int width, int height);

	int createFrameBuffer();
	int createTextureAttachment(int width, int height);
	int createDepthTextureAttachment(int width, int height);
	int createDepthBufferAttachment(int width, int height);

public:
	WaterFrameBuffers(); 
	~WaterFrameBuffers();

	int getReflectionTexture() const { return reflection_texture_; }
	int getRefractionTexture() const { return refraction_texture_; }
	int getRefractionDepthTexture() const { return refraction_depth_texture_; }
};
