#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "objects/Entity.h"
#include "render_engine/MasterRenderer.h"
#include "render_engine/Loader.h"
#include "models/Texture.h"
#include "scene/Scene.h"

using std::string;
using std::map;
using std::shared_ptr;
using std::set;
using std::vector;

class Application {
private:
	MasterRenderer renderer_;
	static shared_ptr<Scene> current_scene;

public:
	static shared_ptr<Loader> loader;
	static shared_ptr<GLFWwindow*> window;
	Application(const shared_ptr<GLFWwindow*>& w);

	// Time keeping for frames
	static int fps_cap;
	static long long previous_frame_time;
	static float frame_delta;
	static long long getCurrentTime();

	void render();

	// Helper functions
	static std::shared_ptr<TexturedModel> makeModel(const string& obj_name, const string& texture_name, const Material& material);
	static std::shared_ptr<TerrainTexturePack> makeTexturePack(const string& base, const string& red, const string& green, const string& blue);
	static void loadPositionsFromFile(const Terrain& terrain, shared_ptr<set<shared_ptr<Entity>>> set, shared_ptr<TexturedModel> model, 
		const std::string& name, vec3 rotation, float scale);

    static void keyCallback(int key, int scan_code, int action, int mods);
    static void mouseButtonCallback(int button, int action, int mods);
    static void cursorPosCallback(double x, double y);
    static void scrollCallBack(double x_offset, double y_offset);
};
