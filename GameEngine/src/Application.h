#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "objects/Entity.h"
#include "objects/Camera.h"
#include "objects/Light.h"
#include "render_engine/MasterRenderer.h"
#include "render_engine/Scene.h"
#include "render_engine/Loader.h"
#include "shaders/DefaultShader.h"

class Application {
private:
	//std::shared_ptr<Player> m_player;

	//Scene m_scene; //Have a scene
	MasterRenderer m_renderer;

	std::vector<Entity> scene;
	Entity m_entity;
	Terrain m_terrain_1;
	Terrain m_terrain_2;

public:
	static GLFWwindow* s_window;
	static Camera s_camera;
	static std::map<char, bool> s_move_keys; 
	static Light s_sun;
	static Loader s_loader;
	static glm::vec3 s_sky_color;

	// Rendering everything
	void render();

	void makeTest();
	std::shared_ptr<TexturedModel> makeModel(const std::string& obj_file,
		const std::string& texture_file, bool transparency, bool fake_lighting);

    void keyCallback(int key, int scancode, int action, int mods);
    void mouseButtonCallback(int button, int action, int mods);
    void cursorPosCallback(double x, double y);
    void scrollCallBack(double x_offset, double y_offset);
};
