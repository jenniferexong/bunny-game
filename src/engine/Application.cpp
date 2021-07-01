#include <GL/glew.h>

#include "Application.h"

#include <GLFW/glfw3.h>

#include "ui/GuiTexture.h"
#include "scene/Scene.h"
#include "IOEvents.h"

using namespace std;

unique_ptr<Application> app = nullptr;

void Application::changeScene(std::weak_ptr<Scene> scene)
{
    current_scene = std::move(scene);
    current_scene.lock()->init();
}

void Application::run(weak_ptr<Scene> initial)
{
    // set up starting scene
    app->changeScene(initial);

    while (!glfwWindowShouldClose(window)) {
        timer->calculateDelta();

        // if scene updated, render
        if (update()) {
			glfwSwapInterval(1);
			glfwSwapBuffers(window);
        }

        timer->update();

        // Poll for and process events 
        glfwPollEvents();
    }

    glfwTerminate();
}

void Application::initialiseWindow() 
{
    /* Initialize the library */
    if (!glfwInit()) {
		Error::exit("GLFW failed init");
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

    GLFWwindow* w = glfwCreateWindow(1280, 720, "", NULL, NULL);
    if (!w) {
		Error::exit("GLFW window failure");
        glfwTerminate();
    }

    glfwMakeContextCurrent(w);

	 // Initialize glew 
    glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		Error::exit("GLEW failed init");
	}

	// number of pixels is x2 on mac
	if (MAC_OS) {
		window_width *= 2;
		window_height *= 2;
	}

    window = w;
}

void Application::create() 
{
	Log::init("Create Application", false);

    app = make_unique<Application>();
    app->initialiseWindow();

	app->loader = std::make_unique<Loader>();
	app->renderer = std::make_unique<MasterRenderer>();
	app->post_processor = std::make_unique<PostProcessor>();
	app->timer = std::make_unique<Timer>();

    glfwSetCursorPosCallback(app->window, IOEvents::cursorPos);
    glfwSetMouseButtonCallback(app->window, IOEvents::mouseButton);
    glfwSetScrollCallback(app->window, IOEvents::scroll);
    glfwSetKeyCallback(app->window, IOEvents::key);
    glfwSetWindowSizeCallback(app->window, IOEvents::windowResize);
    glfwSetFramebufferSizeCallback(app->window, IOEvents::framebufferResize);

	Log::init("Create Application", true);
}

void Application::loadGuiAssets(
    std::vector<string> font_names,
    std::unordered_map<string, int> gui_textures)
{
	app->loadFonts(font_names);
    GuiTexture::loadTextures(gui_textures); // todo move to app
}

void Application::loadFonts(const vector<string>& font_names) {
    for (const auto& name: font_names) {
        fonts_.insert({name , std::make_shared<FontType>(name)});
    } 
}

std::weak_ptr<FontType> Application::getFont(const std::string& name)
{
	return fonts_.at(name);
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

void Application::resizeWindow(int width, int height)
{
	screen_width = width;
	screen_height = height;
}

void Application::resizeFramebuffer(int width, int height)
{
	window_width = width;
	window_height = height;
	aspect_ratio = (float)width / (float)height;
	renderer->resize(width, height);
	post_processor->resizeFbos(width, height);
	glViewport(0, 0, window_width, window_height);
}

void Application::closeWindow() 
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Application::enableCursor(bool enable)
{
	if (enable)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	glfwSetCursorPos(window, screen_width/2.0, screen_height/2.0);
}
