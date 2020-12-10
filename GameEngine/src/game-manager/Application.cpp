#include <GL/glew.h>

#include "Application.h"

#include <GLFW/glfw3.h>

#include "../scene/Scene.h"
#include "../scene/GameScene.h"
#include "../scene/PauseScene.h"
#include "Timer.h"

using namespace std;

unique_ptr<Application> app = nullptr;
unique_ptr<Engine> engine = nullptr;

void Application::changeScene(std::weak_ptr<Scene> scene)
{
    current_scene = std::move(scene);
    current_scene.lock()->init();
}

int Application::run()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    engine = make_unique<Engine>();

    GLFWwindow* window = glfwCreateWindow(engine->window_width, engine->window_height, "", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

	 // Initialize glew 
    glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
        abort();
	}
    engine->init(window);

	app = make_unique<Application>();
    app->init();

    // Setup key and mouse call backs
    glfwSetCursorPosCallback(window, Callbacks::cursorPos);
    glfwSetMouseButtonCallback(window, Callbacks::mouseButton);
    glfwSetScrollCallback(window, Callbacks::scroll);
    glfwSetKeyCallback(window, Callbacks::key);
    glfwSetFramebufferSizeCallback(window, Callbacks::framebufferResize);

    while (!glfwWindowShouldClose(window)) {
		app->timer->calculateDelta();

        // Draw everything
        if (app->update()) {
			glfwSwapInterval(1);
			glfwSwapBuffers(window);
        }
		app->timer->update();

        // Poll for and process events 
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void Application::init()
{
	timer = make_unique<Timer>();
	game_scene = make_shared<GameScene>();
	pause_scene = make_shared<PauseScene>();
    changeScene(game_scene);
}

bool Application::update() {

	return current_scene.lock()->update();
}

void Application::appScrollCallBack(double x_offset, double y_offset)
{
	current_scene.lock()->scrollCallBack(x_offset, y_offset);
}

void Application::appCursorPosCallback(double x, double y)
{
	current_scene.lock()->cursorPosCallback(x, y);
}

void Application::appMouseButtonCallback(int button, int action, int mods)
{
	current_scene.lock()->mouseButtonCallback(button, action, mods);
}

void Application::appKeyCallback(int key, int scan_code, int action, int mods)
{
	current_scene.lock()->keyCallback(key, scan_code, action, mods);
}

void Callbacks::framebufferResize(GLFWwindow* window, int width, int height)
{
    engine->resize(width, height);
}

void Callbacks::cursorPos(GLFWwindow* window, double x, double y)
{
	app->appCursorPosCallback(x, y);
}

void Callbacks::mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	app->appMouseButtonCallback(button, action, mods);
}

void Callbacks::scroll(GLFWwindow* window, double x_offset, double y_offset)
{
	app->appScrollCallBack(x_offset, y_offset);
}

void Callbacks::key(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
	app->appKeyCallback(key, scan_code, action, mods);
}



