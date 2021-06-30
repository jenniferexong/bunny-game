#pragma once

#ifdef __APPLE__
	#define MAC_OS 1
	#define GL_SILENCE_DEPRECATION
#else
	#define MAC_OS 0 
#endif

#include <GLFW/glfw3.h>

#include "Loader.h"
#include "Timer.h"
#include "renderers/MasterRenderer.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class FontType;

class Engine {
private:
	std::unordered_map<std::string, std::shared_ptr<FontType>> fonts_;
public:
    static std::unique_ptr<Engine> instance;

	std::unique_ptr<Loader> loader = nullptr;
	std::unique_ptr<MasterRenderer> renderer = nullptr;
	std::unique_ptr<PostProcessor> post_processor = nullptr;

	std::unique_ptr<Timer> timer;

	GLFWwindow* window = nullptr;
	int window_width = 1280;
	int window_height = 720;
	int screen_width = 1280;
	int screen_height = 720;
	float aspect_ratio = 1280.f / 720.f;

	Engine() = default;

    /**
     * Initializes the engine, given a window
     */
	static void init(GLFWwindow* w);
	void loadFonts();
	std::weak_ptr<FontType> getFont(const std::string& name);

	void resizeWindow(int width, int height);
	void resizeFramebuffer(int width, int height);
	void closeWindow();
	void enableCursor(bool enable);
};
