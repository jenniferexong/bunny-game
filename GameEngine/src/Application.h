#pragma once

#include <GLFW/glfw3.h>

#include <memory>
#include <glm/glm.hpp>

#include "Entity.h"
#include "render_engine/Scene.h"
#include "render_engine/Loader.h"
#include "shaders/DefaultShader.h"

class Scene;
class Player;

class Application {
private:
	static glm::vec2 s_window_size; //do I need this?

	GLFWwindow *m_window;
	std::shared_ptr<Player> m_player;
	Scene m_scene; //Have a scene

	Loader m_loader = Loader();
	DefaultShader m_shader = DefaultShader();

public:

	// Constructor
	Application(GLFWwindow* window, std::shared_ptr<Player> player) : m_window(window), m_player(player) {}

	void makeTest();

	// Rendering everything
	void render();

	void destroy();
};
