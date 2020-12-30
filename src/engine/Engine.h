#pragma once

#ifdef __APPLE__
	#define MAC_OS 1
	#define GL_SILENCE_DEPRECATION
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
	int screen_width = 1280;
	int screen_height = 720;
	float aspect_ratio = 1280.f / 720.f;

	Engine() = default;

	void init(GLFWwindow* w)
	{
		Log::init("Engine", false);

		window = w;

		// number of pixels is x2 on mac
		if (MAC_OS) {
			window_width *= 2;
			window_height *= 2;
		}

		loader = std::make_unique<Loader>();
		renderer = std::make_unique<MasterRenderer>();
		post_processor = std::make_unique<PostProcessor>();

		Log::init("Engine", true);
	}

	void resizeWindow(int width, int height)
	{
		screen_width = width;
		screen_height = height;
	}

	void resizeFramebuffer(int width, int height)
	{
		window_width = width;
		window_height = height;
		aspect_ratio = (float)width / (float)height;
		renderer->resize(width, height);
		post_processor->resizeFbos(width, height);
		glViewport(0, 0, window_width, window_height);
	}

	void closeWindow() 
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void enableCursor(bool enable)
	{
		if (enable)
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

		glfwSetCursorPos(window, screen_width/2.0, screen_height/2.0);
	}
};
