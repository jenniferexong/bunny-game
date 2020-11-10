
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

#include "models/Texture.h"

using namespace glm;
using namespace std;

/* Static variables */
GLFWwindow* Application::window = nullptr;
Camera Application::camera = Camera();
Loader Application::loader = Loader();
vec3 Application::sky_color = vec3(0.039, 0.184, 0.243);
shared_ptr<Player> Application::player = nullptr;
shared_ptr<GuiTexture> Application::compass = nullptr;
vector<Light> Application::lights;
Light Application::sun = Light(vec3(0.f, 100, 1000), vec3(0.4f)); // sun

double Application::previous_mouse_x = 0;
double Application::previous_mouse_y = 0;

// Time keeping
long long Application::previous_frame_time = Application::getCurrentTime();
float Application::frame_delta = 0;

map<Application::Key, bool> Application::move_keys = {
	{Key::W, false}, {Key::A, false}, {Key::S, false}, {Key::D, false}, {Key::Q, false}, {Key::E, false}
};

map<Application::MouseButton, bool> Application::mouse_buttons = {
	{MouseButton::Left, false}, {MouseButton::Right, false}
};

/**
	Method to render everything in the _scene.
	Player position may be updated so need to change the view transform
*/
void Application::render() {
	long long current_frame_time = getCurrentTime();
	frame_delta = float(current_frame_time - previous_frame_time) / 1000.f; // in seconds

	player->updatePosition(terrain_1_);
	camera.updateView();
	renderer_.processEntity(*player);

	// Process all entities
	for (const Entity& e: scene_) 
		renderer_.processEntity(e);

	renderer_.processTerrain(terrain_1_);

	for (const auto& gui: guis_) 
		renderer_.processGui(gui);

	// only render lights that are within a distance from the player
	vector<Light> close_lights = { sun };
	for (const auto& l: lights) {
		if (glm::distance(player->getPosition(), l.getPosition()) <= 250.f)
			close_lights.push_back(l);
	}

	renderer_.render(close_lights);

	previous_frame_time = current_frame_time;
}

void Application::makeTest()
{
	//TODO: Make files that you can read material properties from, and position, scale, rotation...

	int width, height;
	glfwGetWindowSize(Application::window, &width, &height);

	// GUI
	const float padding = 100;
	const float size = 150;
	compass = make_shared<GuiTexture>(loader.loadTexture("res/textures/compass.png"), vec2(padding, height - padding), vec2(size, size));
	guis_.push_back(compass);

	// Terrain
	auto texture_pack = makeTexturePack("default-ground", "light-ground", "blue-ground", "path");
	Texture blend_map = Texture(loader.loadTexture("res/textures/terrain1.png"));
	TerrainTexture ground_texture = TerrainTexture(texture_pack, blend_map);
	terrain_1_ = Terrain(0, -1, ground_texture, "res/textures/height-map.png");

	// Bunny player
	Material white = Material(1.f, 10.f);
	auto player_model = makeModel("bunny", "white", white);
	float player_x = 255.f;
	float player_z = -20.f;
	float player_y = terrain_1_.getHeightOfTerrain(player_x, player_z);
	player = make_shared<Player>(player_model, vec3(player_x, player_y, player_z), vec3(0.f, 0, 0), 1.5f);
	player->setRotationOffset(180.f, 0, 0);
	camera = Camera(player);

	// teapot
	Material teapot_material = Material(1.f, 10.f);
	shared_ptr<TexturedModel> teapot_model = makeModel("teapot", "test", teapot_material);
	Entity teapot = Entity(teapot_model);
	teapot.setPosition(0, 0, -20);
	teapot.setScale(1);
	//_scene.push_back(teapot);

	Material grass_material = Material(true, true);
	shared_ptr<TexturedModel> grass_model = makeModel("grass", "grass-texture", grass_material);

	int num_grass = 100000;
	scene_.reserve(num_grass);
	// Making grass
	for (int i = 0; i < 2000; i++) {
		Entity grass = Entity(grass_model);
		float x = linearRand(0.f, 510.f);
		float z = linearRand(0.f, -510.f);
		float y = terrain_1_.getHeightOfTerrain(x, z);
		grass.setPosition(x, y, z);
		grass.setRotation(linearRand(0.f, 360.f), 0, 0);
		grass.setAlignmentRotation(terrain_1_.getNormalOfTerrain(x, z));
		scene_.push_back(grass);
	}

	Material flower_material = Material(false, false);
	shared_ptr<TexturedModel> flower_model = makeModel("flower", "flower", flower_material);

	float x, y, z;
	string line;

	// reading light positions from file
	ifstream file("res/data/light-positions.txt");
	while (getline(file, line)) {
		Entity flower = Entity(flower_model);

		std::stringstream str_stream(line);
		str_stream >> x >> z;

		y = terrain_1_.getHeightOfTerrain(x, z);
		vec3 terrain_normal = terrain_1_.getNormalOfTerrain(x, z);
		flower.setPosition(x, y, z);
		flower.setRotation(linearRand(0.f, 360.f), -90, 0);
		flower.setAlignmentRotation(terrain_normal);
		flower.setScale(0.15f);
		scene_.push_back(flower);

		// make light slightly above flower to give glow effect
		vec3 color = vec3(1.f, 1, 1);
		vec3 light_pos = flower.getPosition() + (8.f * terrain_normal);
		lights.emplace_back(light_pos, color, Light::point_light_attenuation); // cyan
	}
	file.close();

	Material carrot_material = Material();
	shared_ptr<TexturedModel> carrot_model = makeModel("carrot", "carrot", carrot_material);

	// reading carrot positions from file
	file = ifstream("res/data/carrot-positions.txt");
	while (getline(file, line)) {
		Entity carrot = Entity(carrot_model);

		std::stringstream str_stream(line);
		str_stream >> x >> z;

		y = terrain_1_.getHeightOfTerrain(x, z);
		vec3 terrain_normal = terrain_1_.getNormalOfTerrain(x, z);
		carrot.setPosition(x, y + 0.5, z);
		carrot.setRotation(linearRand(0.f, 360.f), 0, 0);
		carrot.setAlignmentRotation(terrain_normal);
		carrot.setScale(0.02f);
		scene_.push_back(carrot);
	}
	file.close();
}

shared_ptr<TerrainTexturePack> Application::makeTexturePack(const string& base, const string& red,
	const string &green, const string& blue) const
{
	const string prefix("res/textures/");
	Texture base_texture = Texture(loader.loadTexture(prefix + base + ".png"));
	Texture red_texture = Texture(loader.loadTexture(prefix + red + ".png"));
	Texture green_texture = Texture(loader.loadTexture(prefix + green + ".png"));
	Texture blue_texture = Texture(loader.loadTexture(prefix + blue + ".png"));

	return make_shared<TerrainTexturePack>(base_texture, red_texture, green_texture, blue_texture);
}

shared_ptr<TexturedModel> Application::makeModel(const string& obj_name,
	const string& texture_name, const Material& material) const
{
	const string obj_prefix("res/objects/");
	const string texture_prefix("res/textures/");
	InstancedMesh mesh = loader.loadToVao(obj_prefix + obj_name + ".obj");
	ModelTexture texture(loader.loadTexture(texture_prefix + texture_name + ".png"), material);
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
	camera.zoom(float(y_offset));
}

void Application::cursorPosCallback(double x, double y)
{
	player->changeDirection(x - previous_mouse_x);
	if (mouse_buttons[MouseButton::Left]) {
		camera.changePitch(y - previous_mouse_y);
	}
	previous_mouse_x = x;
	previous_mouse_y = y;
}

void Application::mouseButtonCallback(int button, int action, int mods)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) 
			mouse_buttons[MouseButton::Left] = true;
		else if (action == GLFW_RELEASE)
			mouse_buttons[MouseButton::Left] = false;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS) 
			mouse_buttons[MouseButton::Right] = true;
		else if (action == GLFW_RELEASE)
			mouse_buttons[MouseButton::Right] = false;
		break;
	default:
		break;
	}
}

void Application::keyCallback(int key, int scan_code, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_W: 
		if (action == GLFW_PRESS) {
			move_keys[Key::W] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys[Key::W] = false;
		}
		break;
	case GLFW_KEY_A: 
		if (action == GLFW_PRESS) {
			move_keys[Key::A] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys[Key::A] = false;
		}
		break;
	case GLFW_KEY_S: 
		if (action == GLFW_PRESS) {
			move_keys[Key::S] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys[Key::S] = false;
		}
		break;
	case GLFW_KEY_D: 
		if (action == GLFW_PRESS) {
			move_keys[Key::D] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys[Key::D] = false;
		}
		break;
	case GLFW_KEY_Q: 
		if (action == GLFW_PRESS) {
			move_keys[Key::Q] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys[Key::Q] = false;
		}
		break;
	case GLFW_KEY_E: 
		if (action == GLFW_PRESS) {
			move_keys[Key::E] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys[Key::E] = false;
		}
		break;
	case GLFW_KEY_L: 
		if (action == GLFW_RELEASE) {
			string entity = "carrot";
			// Create and open a text file
			ofstream positions("res/data/" + entity + "-positions.txt", ios::app);

			vec3 position = player->getPosition();
			// Write to the file
			positions << position.x << " " << position.z << endl;
		}
		break;
	case GLFW_KEY_SPACE: 
		if (action == GLFW_PRESS) {
			move_keys[Key::Space] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys[Key::Space] = false;
		}
		break;
	case GLFW_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	default:
		break;
	}
}


