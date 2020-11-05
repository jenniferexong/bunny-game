
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <chrono>

#include "models/Texture.h"

using namespace glm;
using namespace std;

/* Static variables */
GLFWwindow* Application::window = nullptr;
Camera Application::camera = Camera();
Light Application::sun = Light(glm::vec3(0.f, 100.f, 1000.f), glm::vec3(1.f));
Loader Application::loader = Loader();
vec3 Application::sky_color = vec3(0.039, 0.184, 0.243);
shared_ptr<Player> Application::player = nullptr;

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

	player->updatePosition();
	camera.updateView();
	renderer_.processEntity(*player);

	// Process all entities
	for (Entity e: scene_) {
		renderer_.processEntity(e);
	}

	renderer_.processTerrain(terrain_1_);

	renderer_.render(sun);

	previous_frame_time = current_frame_time;
}

void Application::makeTest()
{
	//TODO: Make files that you can read material properties from, and position, scale, rotation...

	Material white = Material(1.f, 10.f);
	auto player_model = makeModel("bunny", "white", white);
	player = make_shared<Player>(player_model, vec3(150.f, 0, -20), vec3(0.f, 0, 0), 2.f);
	player->setRotationOffset(180.f, 0, 0);
	camera = Camera(player);

	auto texture_pack = makeTexturePack("default-ground", "light-ground", "blue-ground", "path");
	Texture blend_map = Texture(loader.loadTexture("res/textures/terrain1.png"));
	TerrainTexture ground_texture = TerrainTexture(texture_pack, blend_map);
	terrain_1_ = Terrain(0, -1, ground_texture);

	Material teapot_material = Material(1.f, 10.f);
	shared_ptr<TexturedModel> teapot_model = makeModel("teapot", "test", teapot_material);

	Entity teapot = Entity(teapot_model);
	teapot.setPosition(0, 0, -20);
	teapot.setScale(1);
	//_scene.push_back(teapot);

	Material grass_material = Material(true, true);
	shared_ptr<TexturedModel> grass_model = makeModel("grass-model", "grass-texture", grass_material);

	// Making grass
	for (int i = 0; i < 100; i++) {
		Entity grass = Entity(grass_model);
		grass.setPosition(linearRand(0.f, 300.f), 0, linearRand(0.f, -300.f));
		grass.setScale(linearRand(1.f, 3.f));
		scene_.push_back(grass);
	}
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
	Mesh mesh = loader.loadToVao(obj_prefix + obj_name + ".obj");
	ModelTexture texture(loader.loadTexture(texture_prefix + texture_name + ".png"), material);
	return make_shared<TexturedModel>(mesh, texture);
}

using namespace std::chrono;
long long Application::getCurrentTime()
{
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	return ms.count();
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

void Application::scrollCallBack(double x_offset, double y_offset)
{
	camera.zoom(float(y_offset));
}


