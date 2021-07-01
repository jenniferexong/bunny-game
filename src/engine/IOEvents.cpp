#include "IOEvents.h"
#include "Application.h"

#include "util/Log.h"

void IOEvents::framebufferResize(GLFWwindow* window, int width, int height)
{
	Log::s("RESIZING FRAMEBUFFER");
    app->resizeFramebuffer(width, height);
}

void IOEvents::windowResize(GLFWwindow* window, int width, int height)
{
	Log::s("RESIZING WINDOW");
    app->resizeWindow(width, height);
}

void IOEvents::cursorPos(GLFWwindow* window, double x, double y)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appCursorPosCallback(x, y);
}

void IOEvents::mouseButton(
    GLFWwindow* window, 
    int button, int action, int mods)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appMouseButtonCallback(button, action, mods);
}

void IOEvents::scroll(
    GLFWwindow* window,
    double x_offset, double y_offset)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appScrollCallBack(x_offset, y_offset);
}

void IOEvents::key(GLFWwindow* window, 
    int key, int scan_code, int action, int mods)
{
	if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
		app->appKeyCallback(key, scan_code, action, mods);
}
