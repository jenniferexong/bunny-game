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
#include "shaders/EntityShader.h"
#include "models/Texture.h"
#include "objects/Player.h"
#include "render_engine/GuiRenderer.h"
#include "gui/GuiTexture.h"

using std::string;

class Application {
private:
	MasterRenderer renderer_;
	GuiRenderer gui_renderer_;

	std::vector<Entity> scene_;
	std::vector<std::shared_ptr<GuiTexture>> guis_;
	Terrain terrain_1_;

	static double previous_mouse_x;
	static double previous_mouse_y;

public:
	enum class Key { W, A, S, D, Q, E, Space, Esc };
	enum class MouseButton { Left, Right };

	static GLFWwindow* window;
	static Camera camera;
	static std::shared_ptr<Player> player;
	static std::map<Key, bool> move_keys; 
	static std::map<MouseButton, bool> mouse_buttons; 
	static Light sun;
	static Loader loader;
	static vec3 sky_color;
	static std::shared_ptr<GuiTexture> compass;

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
