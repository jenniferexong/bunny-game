#pragma once

#include "Loader.h"
#include "renderers/MasterRenderer.h"
#include <memory>

class Engine {
public:
	std::unique_ptr<Loader> loader = nullptr;
	std::unique_ptr<MasterRenderer> renderer = nullptr;
	std::unique_ptr<PostProcessor> post_processor = nullptr;

	GLFWwindow* window = nullptr;
	int window_width = 1280;
	int window_height = 720;
	float aspect_ratio = 1280.f / 720.f;

	Engine() = default;

	void init(GLFWwindow* w)
	{
		window = w;
		loader = std::make_unique<Loader>();
		renderer = std::make_unique<MasterRenderer>();
		post_processor = std::make_unique<PostProcessor>();
	}

	void resize(int width, int height)
	{
		window_width = width;
		window_height = height;
		aspect_ratio = (float)width / (float)height;
		renderer->resize(width, height);
		post_processor->resizeFbos(width, height);
		glViewport(0, 0, window_width, window_height);
	}
};
