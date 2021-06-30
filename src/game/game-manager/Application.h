#pragma once

#include <GLFW/glfw3.h>

#include <memory>

#include "engine/Engine.h"
#include "engine/util/Log.h"

class Scene;
class GameScene;
class PauseScene;

class Application {
private:
public:
	std::weak_ptr<Scene> current_scene;

	// scenes
	std::shared_ptr<Scene> game_scene;
	std::shared_ptr<Scene> pause_scene;

	Application() = default;
	void init();
	bool update();
	void changeScene(std::weak_ptr<Scene> scene);

    void appKeyCallback(int key, int scan_code, int action, int mods);
    void appMouseButtonCallback(int button, int action, int mods);
    void appCursorPosCallback(double x, double y);
    void appScrollCallBack(double x_offset, double y_offset);

	static int run();
};

extern std::unique_ptr<Application> app;

namespace Callbacks{
	void cursorPos(GLFWwindow* window, double x, double y);
	void mouseButton(GLFWwindow* window, int button, int action, int mods);
	void scroll(GLFWwindow* window, double x_offset, double y_offset);
	void key(GLFWwindow* window, int key, int scan_code, int action, int mods);
	void framebufferResize(GLFWwindow* window, int width, int height);
	void windowResize(GLFWwindow* window, int width, int height);
}
