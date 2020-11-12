#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <set>

#include "objects/Entity.h"
#include "objects/Camera.h"
#include "objects/Light.h"
#include "render_engine/MasterRenderer.h"
#include "render_engine/Loader.h"
#include "shaders/EntityShader.h"
#include "models/Texture.h"
#include "objects/Player.h"
#include "render_engine/GuiRenderer.h"
#include "gui/GuiTexture.h"
#include "gui/MousePicker.h"

using std::string;
using std::map;
using std::shared_ptr;
using std::set;
using std::vector;

class Application {
private:
	MasterRenderer renderer_;

	map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, CompareTexturedModel> entities_;
	//std::vector<Entity> scene_;
	vector<shared_ptr<GuiTexture>> guis_;
	Terrain terrain_1_;

	static bool first_mouse_movement_;
	static double previous_mouse_x_;
	static double previous_mouse_y_;

	std::shared_ptr<TexturedModel> makeModel(const string& obj_name, const string& texture_name, const Material& material) const;
	std::shared_ptr<TerrainTexturePack> makeTexturePack(const string& base, const string& red, const string& green, const string& blue) const;
	void loadPositionsFromFile(shared_ptr<set<shared_ptr<Entity>>> set, shared_ptr<TexturedModel> model, const std::string& name, vec3 rotation, float scale);

public:
	enum class Key { W, A, S, D, Q, E, L, Space, Esc };
	enum class MouseButton { Left, Right };

	static const float fov;
	static const float near_plane;
	static const float far_plane;

	static GLFWwindow* window;
	static Camera camera;
	static MousePicker mouse_picker;

	static std::shared_ptr<Player> player;
	static std::map<Key, bool> move_keys; 
	static std::map<MouseButton, bool> mouse_buttons; 
	static Light sun;
	static std::vector<Light> lights;
	static Loader loader;
	static vec3 fog_color;
	static std::shared_ptr<GuiTexture> compass;

	// Time keeping for frames
	static int fps_cap;
	static long long previous_frame_time;
	static float frame_delta;

	// Rendering everything
	void render();

	void setup();
	void makeGame();
	void makeTest();

	static glm::mat4 getProjectionMatrix();

	static long long getCurrentTime();

    static void keyCallback(int key, int scan_code, int action, int mods);
    static void mouseButtonCallback(int button, int action, int mods);
    static void cursorPosCallback(double x, double y);
    static void scrollCallBack(double x_offset, double y_offset);
};
