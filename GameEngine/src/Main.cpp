
// External Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h> BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(GLFWwindow)
#include <stb_image/stb_image.h>

// std
#include <iostream>
#include <memory>

#include "Application.h"
#include "Location.h"

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

    GLFWwindow* window = glfwCreateWindow(1280, 720, "", NULL, NULL);
    /*
    std::string file_path = FilePath::texture_path + "cursor.png";
    int width, height, bpp; // bits per pixel
    unsigned char* buffer = stbi_load(file_path.c_str(), &width, &height, &bpp, 4);
    GLFWimage cursor_image{ width, height, buffer };
    GLFWcursor* cursor = glfwCreateCursor(&cursor_image, 5, 5);
    glfwSetCursor(window, cursor);
    */

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
        abort();
	}

    // Make the application
    Application application(make_shared<GLFWwindow*>(window));

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

		glfwSwapInterval(1);
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

