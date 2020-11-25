
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

#include "Location.h"

#include "models/Texture.h"
#include "scene/GameScene.h"
#include "scene/TestScene.h"

using namespace glm;
using namespace std;

/* Static variables */
shared_ptr<Loader> Application::loader = make_shared<Loader>();
shared_ptr<Scene> Application::current_scene = nullptr;
shared_ptr<GLFWwindow*> Application::window = nullptr;
int Application::frame_count_ = 0;
float Application::delta_time_ = 0.f;
float Application::update_rate_ = 4.f;
float Application::fps = 0.f;


// Time keeping
long long Application::previous_frame_time = Application::getCurrentTime();
float Application::frame_delta = 0;

void Application::updateFps()
{
	frame_count_++;
	delta_time_ += frame_delta;
	if (delta_time_ > 1.f / update_rate_) {
		fps = (float) frame_count_ / delta_time_;
		frame_count_ = 0;
		delta_time_ -= 1.f / update_rate_;
	}
}

Application::Application(const shared_ptr<GLFWwindow*>& w)
{
	renderer_ = make_shared<MasterRenderer>();
	window = w;
	//current_scene = make_shared<GameScene>(renderer_, window, loader);
	current_scene = make_shared<TestScene>(renderer_);
}

void Application::render() {
	long long current_frame_time = getCurrentTime();
	frame_delta = float(current_frame_time - previous_frame_time) / 1000.f; // in seconds
	updateFps();

	current_scene->update();
	current_scene->render();
	current_scene->postRenderUpdate();

	previous_frame_time = current_frame_time;
}

void Application::loadPositionsFromFile(const Terrain& terrain, Environment& environment, shared_ptr<TexturedModel> model, const std::string& name, vec3 rotation, float scale)
{
	float x, y, z;
	string line;

	ifstream file(FilePath::data_path + name + "-positions.txt");
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

void Application::loadWaterFromFile(Environment& environment, float water_height)
{
	const string file_name = FilePath::data_path + "water-positions.txt";
	float x, z;
	string line;

	ifstream file(file_name);
	while (getline(file, line)) {
		std::stringstream str_stream(line);
		str_stream >> x >> z;

		Water water = Water(x, z, water_height);
		environment.addWater(water);
	}
	file.close();
}

void Application::loadPositionsFromFileToSet(const Terrain& terrain, shared_ptr<set<shared_ptr<Entity>>> set, shared_ptr<TexturedModel> model, const std::string& name, vec3 rotation, float scale)
{
	float x, y, z;
	string line;

	ifstream file(FilePath::data_path + name + "-positions.txt");
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
	Texture base_texture = Texture(loader->loadTexture(base));
	Texture red_texture = Texture(loader->loadTexture(red));
	Texture green_texture = Texture(loader->loadTexture(green));
	Texture blue_texture = Texture(loader->loadTexture(blue));

	return make_shared<TerrainTexturePack>(base_texture, red_texture, green_texture, blue_texture);
}

shared_ptr<TexturedModel> Application::makeModel(const string& obj_name,
	const string& texture_name, const Material& material)
{
	InstancedMesh mesh = loader->loadToVaoInstanced(obj_name);
	ModelTexture texture(loader->loadTexture(texture_name), material);
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


