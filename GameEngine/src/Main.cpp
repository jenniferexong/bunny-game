
// External Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

// std
#include <iostream>
#include <memory>

#include "Application.h"

using namespace std;
using namespace glm;

/* GLFW callbacks */
namespace {
    void cursorPosCallback(GLFWwindow* window, double x, double y);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallBack(GLFWwindow* window, double x_offset, double y_offset);
    void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods);

    Application* application_ptr = nullptr;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    Application::window = glfwCreateWindow(1280, 720, "", NULL, NULL);
    GLFWwindow* window = Application::window;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        cout << "!1" << endl;
        //abort();
	}

    // Make the application
    Application application;
    application.makeTest();

    application_ptr = &application;

    // Setup key and mouse call backs
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallBack);
    glfwSetKeyCallback(window, keyCallback);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window)) {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw everything
        application.render();

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

namespace {
    void cursorPosCallback(GLFWwindow* window, double x, double y)
    {
        application_ptr->cursorPosCallback(x, y);
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        application_ptr->mouseButtonCallback(button, action, mods);
    }

    void scrollCallBack(GLFWwindow* window, double x_offset, double y_offset)
    {
        application_ptr->scrollCallBack(x_offset, y_offset);
    }

    void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods)
    {
        application_ptr->keyCallback(key, scan_code, action, mods);
    }
}

