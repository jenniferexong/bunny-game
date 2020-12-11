#pragma once

#ifdef __APPLE__
	#define MAC_OS 1
#else
	#define MAC_OS 0 
#endif

#include <GLFW/glfw3.h>

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
		Print::init("Engine", false);

		window = w;

		// number of pixels is x2 on mac
		if (MAC_OS) {
			Print::s("MAC OS");
			window_width *= 2;
			window_height *= 2;
		}

		loader = std::make_unique<Loader>();
		renderer = std::make_unique<MasterRenderer>();
		post_processor = std::make_unique<PostProcessor>();

		Print::init("Engine", true);
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
