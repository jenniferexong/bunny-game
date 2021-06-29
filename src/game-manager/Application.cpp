#include <GL/glew.h>

#include "Application.h"

#include <GLFW/glfw3.h>

#include "../scene/Scene.h"
#include "../scene/GameScene.h"
#include "../scene/PauseScene.h"
#include "Timer.h"

using namespace std;

unique_ptr<Application> app = nullptr;

void Application::changeScene(std::weak_ptr<Scene> scene)
{
    current_scene = std::move(scene);
    current_scene.lock()->init();
}

int Application::run()
{
    /* Initialize the library */
    if (!glfwInit()) {
		Error::exit("GLFW failed init");
        return -1;
	}

    // force OpenGL to create a 3.3 core context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// disallow legacy functionality (helps OS X work)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

	// get the version for GLFW for later
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "", NULL, NULL);
    if (!window) {
		Error::exit("GLFW window failure");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	 // Initialize glew 
    glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		Error::exit("GLEW failed init");
        abort();
	}

    Engine::init(window);

	app = make_unique<Application>();
    app->init();

    glfwSetCursorPosCallback(window, Callbacks::cursorPos);
    glfwSetMouseButtonCallback(window, Callbacks::mouseButton);
    glfwSetScrollCallback(window, Callbacks::scroll);
    glfwSetKeyCallback(window, Callbacks::key);
    glfwSetWindowSizeCallback(window, Callbacks::windowResize);
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

	printf("\n");
    glfwTerminate();
    return 0;
}

void Application::init()
{
	Log::init("Application", false);
	timer = make_unique<Timer>();
	game_scene = make_shared<GameScene>();
	pause_scene = make_shared<PauseScene>();
    changeScene(game_scene);
	Log::init("Application", true);
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
	Log::s("RESIZING FRAMEBUFFER");
    Engine::instance->resizeFramebuffer(width, height);
}

void Callbacks::windowResize(GLFWwindow* window, int width, int height)
{
	Log::s("RESIZING WINDOW");
    Engine::instance->resizeWindow(width, height);
}

void Callbacks::cursorPos(GLFWwindow* window, double x, double y)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appCursorPosCallback(x, y);
}

void Callbacks::mouseButton(
		GLFWwindow* window, 
		int button, int action, int mods)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appMouseButtonCallback(button, action, mods);
}

void Callbacks::scroll(
		GLFWwindow* window,
		double x_offset, double y_offset)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appScrollCallBack(x_offset, y_offset);
}

void Callbacks::key(GLFWwindow* window, 
		int key, int scan_code, int action, int mods)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appKeyCallback(key, scan_code, action, mods);
}



