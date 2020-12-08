#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <sstream>
#include <chrono>

#include "Helper.h"

#include "renderers/MasterRenderer.h"
#include "environment/Player.h"
#include "Location.h"
#include "models/Texture.h"
#include "scene/GameScene.h"
#include "Loader.h"

using namespace glm;
using namespace std;

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

Application::Application(GLFWwindow* w): window(w)
{
}

void Application::initialize()
{
	loader = make_unique<Loader>();
	renderer = make_unique<MasterRenderer>();
	game_scene = make_shared<GameScene>();
	current_scene = game_scene;
	previous_frame_time = getCurrentTime();
}

void Application::render() {
	long long current_frame_time = getCurrentTime();
	frame_delta = float(current_frame_time - previous_frame_time) / 1000.f; // in seconds
	updateFps();

	current_scene.lock()->update();
	current_scene.lock()->render();
	current_scene.lock()->postRenderUpdate();

	previous_frame_time = current_frame_time;
}

void Application::spawnEntity(shared_ptr<Player> player, Environment& environment, TexturedModel& model, const std::string& name, vec3 rotation, float scale)
{
	// write position to file
	std::ofstream positions(FilePath::data_path + name + "-positions.txt", std::ios::app);
	vec3 position = player->getPosition();
	auto entity = make_shared<Entity>(model);

	positions << position.x << " " << position.z << std::endl;
	positions.close();

	Terrain terrain = environment.getTerrain(player);
	float y = terrain.getHeightOfTerrain(position.x, position.z);
	vec3 terrain_normal = terrain.getNormalOfTerrain(position.x, position.z);
	entity->setPosition(position.x, y, position.z);
	entity->setRotation(linearRand(0.f, 90.f), 0, 0);
	entity->rotate(rotation.x, rotation.y, rotation.z);
	entity->setAlignmentRotation(terrain_normal);
	entity->setScale(scale);
	environment.addEntity(entity);
}

void Application::loadPositionsFromFile(const Terrain& terrain, Environment& environment, TexturedModel& model, const std::string& name, vec3 rotation, float scale)
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

void Application::loadPositionsFromFileToSet(const Terrain& terrain, set<shared_ptr<Entity>>& set, TexturedModel& model,
	const std::string& name, vec3 rotation, float scale)
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
		set.insert(entity);
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

TexturedModel Application::makeModel(const string& obj_name,
	const string& texture_name, const Material& material)
{
	InstancedMesh mesh = loader->loadToVaoInstanced(obj_name);
	ModelTexture texture(loader->loadTexture(texture_name), material);
	return TexturedModel(mesh, texture);
}

using namespace std::chrono;
long long Application::getCurrentTime()
{
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	return ms.count();
}

void Application::appScrollCallBack(double x_offset, double y_offset)
{
	current_scene.lock()->scrollCallBack(x_offset, y_offset);
}

void Application::appCursorPosCallback(double x, double y)
{
	current_scene.lock()->cursorPosCallback(x, y);
}

void Application::appMouseButtonCallback(int button, int action, int mods)
{
	current_scene.lock()->mouseButtonCallback(button, action, mods);
}

void Application::appKeyCallback(int key, int scan_code, int action, int mods)
{
	current_scene.lock()->keyCallback(key, scan_code, action, mods);
}

// ----------------------------------------------------------------------

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
	app->appCursorPosCallback(x, y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	app->appMouseButtonCallback(button, action, mods);
}

void scrollCallBack(GLFWwindow* window, double x_offset, double y_offset)
{
	app->appScrollCallBack(x_offset, y_offset);
}

void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
	app->appKeyCallback(key, scan_code, action, mods);
}

unique_ptr<Application> app = nullptr;
int Application::run()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

	 // Initialize glew 
    glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
        abort();
	}

    // Make the application
	app = make_unique<Application>(window);
	app->initialize();
	Print::s("APP INITIALIZED");

    // Setup key and mouse call backs
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallBack);
    glfwSetKeyCallback(window, keyCallback);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window)) {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw everything
        app->render();

		glfwSwapInterval(1);
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




