#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "objects/Entity.h"
#include "objects/Camera.h"
#include "objects/Light.h"
#include "render_engine/MasterRenderer.h"
#include "render_engine/Loader.h"
#include "shaders/DefaultShader.h"
#include "models/Texture.h"
#include "objects/Player.h"

using std::string;

class Application {
private:
	//std::shared_ptr<Player> m_player;

	MasterRenderer _renderer;

	Player _player;
	std::vector<Entity> _scene;
	Entity _entity;
	Terrain _terrain_1;
	Terrain _terrain_2;

public:
	enum class Key { W, A, S, D, Q, E, Space };

	static GLFWwindow* window;
	static Camera camera;
	static std::map<Key, bool> move_keys; 
	static Light sun;
	static Loader loader;
	static vec3 sky_color;

	// Time keeping for frames
	static int fps_cap;
	static long long previous_frame_time;
	static float frame_delta;

	// Rendering everything
	void render();

	void makeTest();
	std::shared_ptr<TexturedModel> makeModel(const string& obj_name, const string& texture_name, const Material& material) const;
	std::shared_ptr<TerrainTexturePack> makeTexturePack(const string& base, const string& red, const string& green, const string& blue) const;

	static long long getCurrentTime();

    static void keyCallback(int key, int scan_code, int action, int mods);
    static void mouseButtonCallback(int button, int action, int mods);
    static void cursorPosCallback(double x, double y);
    static void scrollCallBack(double x_offset, double y_offset);
};
