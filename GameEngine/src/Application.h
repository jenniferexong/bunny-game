#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "object/Entity.h"
#include "object/Camera.h"
#include "object/Light.h"
#include "render_engine/Renderer.h"
#include "render_engine/Scene.h"
#include "render_engine/Loader.h"
#include "shaders/DefaultShader.h"

class Application {
private:
	//std::shared_ptr<Player> m_player;

	//Scene m_scene; //Have a scene
	MasterRenderer m_renderer;

	Entity m_entity;

public:
	static GLFWwindow* s_window;
	static Camera s_camera;
	static std::map<char, bool> s_move_keys; 
	static Light s_sun;
	static Loader s_loader;

	void setUp();
	void makeTest();

	// Rendering everything
	void render();

    void keyCallback(int key, int scancode, int action, int mods);
    void mouseButtonCallback(int button, int action, int mods);
    void cursorPosCallback(double x, double y);
    void scrollCallBack(double x_offset, double y_offset);
};
