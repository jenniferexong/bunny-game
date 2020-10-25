#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>


class Entity;
class Shader;

class Renderer {
private:
	static const float s_fov;
	static const float s_near_plane;
	static const float s_far_plane;
	GLFWwindow* m_window;

	glm::mat4 getProjectionMatrix();

public:
	Renderer() : m_window(nullptr) {}

	Renderer(GLFWwindow* window) : m_window(window) {}

	void prepare();
	void render(const Entity& entity, const Shader& shader);
};

