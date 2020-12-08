#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

class MasterRenderer;
class Scene;
class GameScene;
class Loader;
class TexturedModel;
class TerrainTexturePack;
class Environment;
class Terrain;
class Entity;
class Player;
struct Material;

using std::string;
using std::map;
using std::unique_ptr;
using std::shared_ptr;
using std::set;
using std::vector;
using glm::vec3;

class GameScene;

class Application {
private:
	int frame_count_ = 0;
	float delta_time_= 0.f;
	float update_rate_ = 4.f;
	void updateFps();

public:
	unique_ptr<Loader> loader = nullptr;
	unique_ptr<MasterRenderer> renderer = nullptr;
	GLFWwindow* window;
	std::weak_ptr<Scene> current_scene;

	// scenes
	shared_ptr<GameScene> game_scene = nullptr;
	//static unique_ptr<PauseScene> pause_scene;

	Application(GLFWwindow* w);
	void initialize();
	void render();

	long long previous_frame_time;
	float frame_delta = 0.f;
	float fps = 0.f;

	long long getCurrentTime();

	// Helper functions
	TexturedModel makeModel(const string& obj_name, const string& texture_name, const Material& material);
	std::shared_ptr<TerrainTexturePack> makeTexturePack(const string& base, const string& red, const string& green, const string& blue);
	void loadPositionsFromFile(const Terrain& terrain, Environment& environment, TexturedModel& model, 
		const std::string& name, vec3 rotation, float scale);

	void loadPositionsFromFileToSet(const Terrain& terrain, set<shared_ptr<Entity>>& set, TexturedModel& model, 
		const std::string& name, vec3 rotation, float scale); // TODO

	void spawnEntity(shared_ptr<Player> player, Environment& environment, TexturedModel& model, const std::string& name, vec3 rotation, float scale);

    void appKeyCallback(int key, int scan_code, int action, int mods);
    void appMouseButtonCallback(int button, int action, int mods);
    void appCursorPosCallback(double x, double y);
    void appScrollCallBack(double x_offset, double y_offset);

	static int run();
};

extern unique_ptr<Application> app;


