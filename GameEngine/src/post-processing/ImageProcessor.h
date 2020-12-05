#pragma once

#include <memory>

#include "ImageRenderer.h"
#include "../shaders/Shader.h"

class ImageProcessor {
private:
	std::shared_ptr<Shader> shader_;
	ImageRenderer renderer_;

public:
	ImageProcessor(std::shared_ptr<Shader> shader, ImageRenderer renderer);

	void render(int texture);
};
