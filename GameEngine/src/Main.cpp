
// External Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

// std
#include <iostream>
#include <memory>

#include "Player.h"
#include "Application.h"

using namespace std;
using namespace glm;

int main(void) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
        abort();
	}

    // Make the player
    shared_ptr<Player> player = make_shared<Player>(vec3(0.f));

    // Make the application
    Application application = Application(window, player);
    application.makeTest();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw everything
        application.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    application.destroy(); // clean everything up
    glfwTerminate();
    return 0;
}

