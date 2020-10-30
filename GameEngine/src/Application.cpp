
#include "Application.h"

#include <glm/glm.hpp>

#include <iostream>

#include "render_engine/Texture.h"
#include "render_engine/Camera.h"
#include "Light.h"

using namespace glm;
using namespace std;

/* Static variables */
GLFWwindow* Application::s_window = nullptr;
Camera Application::s_camera = Camera();
Light Application::s_light = Light(glm::vec3(0.f, 100.f, 1000.f), glm::vec3(1.f));
map<char, bool> Application::s_move_keys = {{'w', false}, {'a', false}, {'s', false}, {'d', false}};

/**
	Method to render everything in the scene.
	Player position may be updated so need to change the view transform
*/
void Application::render() {
	// animate
	//m_entity.move(0, 0, -0.1f);
	m_entity.rotate(0.1f, 0, 0);

	s_camera.updatePosition();

	// Process all entities
	m_renderer.processEntity(m_entity);

	m_renderer.render(s_light);
}

/* Initialize all the member variables properly */
void Application::setUp() {
	m_loader = Loader();
	DefaultShader shader;
	m_renderer = MasterRenderer(shader);
}

void Application::makeTest() {
	//Mesh mesh = m_loader.loadToVao(positions, texture_coords, indices);
	Mesh mesh = m_loader.loadToVao("res/objects/teapot.obj");

	ModelTexture texture = ModelTexture(m_loader.loadTexture("res/textures/checkerboard.png"));
	texture.setShineDamper(10.f);
	texture.setReflectivity(1.f);
	shared_ptr<TexturedModel> textured_model = make_shared<TexturedModel>(mesh, texture);

	m_entity = Entity(textured_model);
	m_entity.setPosition(0, -5, -20);
	m_entity.setScale(1);

	//shared_ptr<Entity> entity = make_shared<Entity>(textured_model);
	//m_scene.addToScene(entity);
}

void Application::keyCallback(int key, int scancode, int action, int mods) {
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
	}
}

void Application::cursorPosCallback(double x, double y) {

}

void Application::mouseButtonCallback(int button, int action, int mods) {

}

void Application::scrollCallBack(double x_offset, double y_offset) {

}


