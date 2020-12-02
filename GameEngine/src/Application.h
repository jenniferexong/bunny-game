#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <map>
#include <memory>

#include "renderers/MasterRenderer.h"
#include "Loader.h"
#include "environment/Entity.h"
#include "models/Texture.h"
#include "scene/Scene.h"

using std::string;
using std::map;
using std::shared_ptr;
using std::set;
using std::vector;

class Application {
private:
	shared_ptr<MasterRenderer> renderer_;
	static shared_ptr<Scene> current_scene;

	static int frame_count_;
	static float delta_time_;
	static float update_rate_;
	void updateFps();

public:
	static shared_ptr<Loader> loader;
	static shared_ptr<GLFWwindow*> window;
	Application(const shared_ptr<GLFWwindow*>& w);

	static long long previous_frame_time;
	static float frame_delta;
	static float fps;
	static long long getCurrentTime();

	void render();

	// Helper functions
	static std::shared_ptr<TexturedModel> makeModel(const string& obj_name, const string& texture_name, const Material& material);
	static std::shared_ptr<TerrainTexturePack> makeTexturePack(const string& base, const string& red, const string& green, const string& blue);
	static void loadPositionsFromFile(const Terrain& terrain, Environment& environment, shared_ptr<TexturedModel> model, 
		const std::string& name, vec3 rotation, float scale);
	static void loadPositionsFromFileToSet(const Terrain& terrain, shared_ptr<set<shared_ptr<Entity>>> set, shared_ptr<TexturedModel> model, 
		const std::string& name, vec3 rotation, float scale);
	static void loadWaterFromFile(Environment& environment, float water_height);
	static void spawnEntity(shared_ptr<Player> player, Environment& environment, shared_ptr<TexturedModel> model, const std::string& name, vec3 rotation, float scale);

    static void keyCallback(int key, int scan_code, int action, int mods);
    static void mouseButtonCallback(int button, int action, int mods);
    static void cursorPosCallback(double x, double y);
    static void scrollCallBack(double x_offset, double y_offset);
};
