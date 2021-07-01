#include <GLFW/glfw3.h>

namespace IOEvents {
	void cursorPos(GLFWwindow* window, double x, double y);
	void mouseButton(GLFWwindow* window, int button, int action, int mods);
	void scroll(GLFWwindow* window, double x_offset, double y_offset);
	void key(GLFWwindow* window, int key, int scan_code, int action, int mods);
	void framebufferResize(GLFWwindow* window, int width, int height);
	void windowResize(GLFWwindow* window, int width, int height);
}
