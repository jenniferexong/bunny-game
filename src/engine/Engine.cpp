#include "Engine.h"
#include "text/FontType.h"
#include "../ui/GuiTexture.h"

void Engine::init(GLFWwindow* w)
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

	loadFonts();
    GuiTexture::loadTextures();

	Log::init("Engine", true);
}

void Engine::loadFonts()
{
	const std::vector<std::string> font_names = {
		"maiandra"
	};

	for (const auto& name: font_names)
		fonts_.insert({name , std::make_shared<FontType>(name)});
}

std::weak_ptr<FontType> Engine::getFont(const std::string& name)
{
	return fonts_.at(name);
}

void Engine::resizeWindow(int width, int height)
{
	screen_width = width;
	screen_height = height;
}

void Engine::resizeFramebuffer(int width, int height)
{
	window_width = width;
	window_height = height;
	aspect_ratio = (float)width / (float)height;
	renderer->resize(width, height);
	post_processor->resizeFbos(width, height);
	glViewport(0, 0, window_width, window_height);
}

void Engine::closeWindow() 
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Engine::enableCursor(bool enable)
{
	if (enable)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	glfwSetCursorPos(window, screen_width/2.0, screen_height/2.0);
}
