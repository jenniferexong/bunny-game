#include <GL/glew.h>

#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(std::shared_ptr<Shader> shader, ImageRenderer renderer) : renderer_(renderer)
{
	shader_ = std::move(shader);
	shader_->setUp();
}

void ImageProcessor::render(int texture)
{
	shader_->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	renderer_.renderQuad();
	shader_->stop();
}

