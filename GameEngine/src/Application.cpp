
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

#include "models/Texture.h"

#include "scene/GameScene.h"

using namespace glm;
using namespace std;

/* Static variables */
shared_ptr<Loader> Application::loader = make_shared<Loader>();
shared_ptr<Scene> Application::current_scene = nullptr;
shared_ptr<GLFWwindow*> Application::window = nullptr;

// Time keeping
long long Application::previous_frame_time = Application::getCurrentTime();
float Application::frame_delta = 0;

Application::Application(const shared_ptr<GLFWwindow*>& w)
{
	window = w;
	current_scene = make_shared<GameScene>(window, loader);
}

void Application::render() {
	long long current_frame_time = getCurrentTime();
	frame_delta = float(current_frame_time - previous_frame_time) / 1000.f; // in seconds

	current_scene->update();
	renderer_.renderScene(current_scene);
	current_scene->postRenderUpdate();

	previous_frame_time = current_frame_time;
}

void Application::loadPositionsFromFile(const Terrain& terrain, Environment& environment, shared_ptr<TexturedModel> model, const std::string& name, vec3 rotation, float scale)
{
	float x, y, z;
	string line;

	ifstream file("res/data/" + name + "-positions.txt");
	while (getline(file, line)) {
		auto entity = make_shared<Entity>(model);

		std::stringstream str_stream(line);
		str_stream >> x >> z;

		y = terrain.getHeightOfTerrain(x, z);
		vec3 terrain_normal = terrain.getNormalOfTerrain(x, z);
		entity->setPosition(x, y, z);
		entity->setRotation(linearRand(0.f, 90.f), 0, 0);
		entity->rotate(rotation.x, rotation.y, rotation.z);
		entity->setAlignmentRotation(terrain_normal);
		entity->setScale(scale);
		environment.addEntity(entity);
	}
	file.close();
}

void Application::loadPositionsFromFileToSet(const Terrain& terrain, shared_ptr<set<shared_ptr<Entity>>> set, shared_ptr<TexturedModel> model, const std::string& name, vec3 rotation, float scale)
{
	float x, y, z;
	string line;

	ifstream file("res/data/" + name + "-positions.txt");
	while (getline(file, line)) {
		auto entity = make_shared<Entity>(model);

		std::stringstream str_stream(line);
		str_stream >> x >> z;

		y = terrain.getHeightOfTerrain(x, z);
		vec3 terrain_normal = terrain.getNormalOfTerrain(x, z);
		entity->setPosition(x, y, z);
		entity->setRotation(linearRand(0.f, 90.f), 0, 0);
		entity->rotate(rotation.x, rotation.y, rotation.z);
		entity->setAlignmentRotation(terrain_normal);
		entity->setScale(scale);
		set->insert(entity);
	}
	file.close();
}

shared_ptr<TerrainTexturePack> Application::makeTexturePack(const string& base, const string& red,
	const string &green, const string& blue) 
{
	const string prefix("res/textures/");
	Texture base_texture = Texture(loader->loadTexture(prefix + base + ".png"));
	Texture red_texture = Texture(loader->loadTexture(prefix + red + ".png"));
	Texture green_texture = Texture(loader->loadTexture(prefix + green + ".png"));
	Texture blue_texture = Texture(loader->loadTexture(prefix + blue + ".png"));

	return make_shared<TerrainTexturePack>(base_texture, red_texture, green_texture, blue_texture);
}

shared_ptr<TexturedModel> Application::makeModel(const string& obj_name,
	const string& texture_name, const Material& material)
{
	const string obj_prefix("res/objects/");
	const string texture_prefix("res/textures/");
	InstancedMesh mesh = loader->loadToVaoInstanced(obj_prefix + obj_name + ".obj");
	ModelTexture texture(loader->loadTexture(texture_prefix + texture_name + ".png"), material);
	return make_shared<TexturedModel>(mesh, texture);
}

using namespace std::chrono;
long long Application::getCurrentTime()
{
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	return ms.count();
}

void Application::scrollCallBack(double x_offset, double y_offset)
{
	current_scene->scrollCallBack(x_offset, y_offset);
}

void Application::cursorPosCallback(double x, double y)
{
	current_scene->cursorPosCallback(x, y);
}

void Application::mouseButtonCallback(int button, int action, int mods)
{
	current_scene->mouseButtonCallback(button, action, mods);
}

void Application::keyCallback(int key, int scan_code, int action, int mods)
{
	current_scene->keyCallback(key, scan_code, action, mods);
}


