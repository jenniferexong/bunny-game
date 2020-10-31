
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>

#include "models/Texture.h"

using namespace glm;
using namespace std;

/* Static variables */
GLFWwindow* Application::s_window = nullptr;
Camera Application::s_camera = Camera();
Light Application::s_sun = Light(glm::vec3(0.f, 100.f, 1000.f), glm::vec3(1.f));
Loader Application::s_loader = Loader();
vec3 Application::s_sky_color = vec3(0.352f, 0.686f, 0.807f);

map<char, bool> Application::s_move_keys = {
	{'w', false}, {'a', false}, {'s', false}, {'d', false}, {'q', false}, {'e', false}
};

/**
	Method to render everything in the scene.
	Player position may be updated so need to change the view transform
*/
void Application::render() {
	// animate
	//m_entity.move(0, 0, -0.1f);

	//e.rotate(0.1f, 0, 0);

	s_camera.updatePosition();

	// Process all entities
	for (Entity e : scene) {
		m_renderer.processEntity(e);
	}

	m_renderer.processTerrain(m_terrain_1);
	m_renderer.processTerrain(m_terrain_2);

	m_renderer.render(s_sun);
}

void Application::makeTest()
{
	//TODO: Make files that you can read material properties from, and position, scale, rotation...

	ModelTexture ground = ModelTexture(s_loader.loadTexture("res/textures/ground.png"));
	m_terrain_1 = Terrain(0, -1, ground);
	m_terrain_2 = Terrain(-1, -1, ground);

	shared_ptr<TexturedModel> teapot_model = makeModel("teapot", "test", false, false);
	teapot_model->setShineValues(1.f, 10.f);

	Entity teapot = Entity(teapot_model);
	teapot.setPosition(0, 0, -20);
	teapot.setScale(1);
	scene.push_back(teapot);

	shared_ptr<TexturedModel> grass_model = makeModel("grass-model", "grass-texture", true, true);

	// Making grass
	for (int i = 0; i < 50; i++) {
		Entity grass = Entity(grass_model);
		grass.setPosition(glm::linearRand(-30.f, 30.f), 0, glm::linearRand(-50.f, 0.f));
		scene.push_back(grass);
	}
}

shared_ptr<TexturedModel> Application::makeModel(const std::string& obj_name,
	const std::string& texture_name, bool transparency, bool fake_lighting)
{
	string obj_prefix("res/objects/");
	string texture_prefix("res/textures/");
	Mesh mesh = s_loader.loadToVao(obj_prefix + obj_name + ".obj");
	ModelTexture texture(s_loader.loadTexture(texture_prefix + texture_name + ".png"));
	texture.setTransparent(transparency);
	texture.setFakeLighting(fake_lighting);
	return make_shared<TexturedModel>(mesh, texture);
}

void Application::keyCallback(int key, int scancode, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_W: 
		if (action == GLFW_PRESS) {
			s_move_keys['w'] = true;
		} else if(action == GLFW_RELEASE) {
			s_move_keys['w'] = false;
		}
		break;
	case GLFW_KEY_A: 
		if (action == GLFW_PRESS) {
			s_move_keys['a'] = true;
		} else if(action == GLFW_RELEASE) {
			s_move_keys['a'] = false;
		}
		break;
	case GLFW_KEY_S: 
		if (action == GLFW_PRESS) {
			s_move_keys['s'] = true;
		} else if(action == GLFW_RELEASE) {
			s_move_keys['s'] = false;
		}
		break;
	case GLFW_KEY_D: 
		if (action == GLFW_PRESS) {
			s_move_keys['d'] = true;
		} else if(action == GLFW_RELEASE) {
			s_move_keys['d'] = false;
		}
		break;
	case GLFW_KEY_Q: 
		if (action == GLFW_PRESS) {
			s_move_keys['q'] = true;
		} else if(action == GLFW_RELEASE) {
			s_move_keys['q'] = false;
		}
		break;
	case GLFW_KEY_E: 
		if (action == GLFW_PRESS) {
			s_move_keys['e'] = true;
		} else if(action == GLFW_RELEASE) {
			s_move_keys['e'] = false;
		}
		break;
	}
}

void Application::cursorPosCallback(double x, double y)
{

}

void Application::mouseButtonCallback(int button, int action, int mods)
{

}

void Application::scrollCallBack(double x_offset, double y_offset)
{

}


