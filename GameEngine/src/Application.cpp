
#include "Application.h"

#include <glm/glm.hpp>

#include <iostream>

#include "render_engine/Texture.h"

using namespace glm;
using namespace std;

// Setting static variables
vec2 Application::s_window_size = vec2(1280, 720);

/**
	Method to render everything in the scene.
	Player position may be updated so need to change the view transform
*/
void Application::render() {
	m_shader.start();
	m_scene.render(m_window, m_player);
	m_shader.stop();
}

void Application::makeTest() {

	// Making test position data
	vector<float> positions = {
		-0.5f, 0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.5f, 0.5f, 0.f
	};

	vector<float> texture_coords = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f
	};

	vector<int> indices = {
		0, 1, 3, 3, 1, 2
	};

	Mesh mesh = m_loader.loadToVao(positions, texture_coords, indices);

	ModelTexture texture = ModelTexture(m_loader.loadTexture("res/textures/checkerboard.png"));
	shared_ptr<Model> textured_model = make_shared<TexturedModel>(mesh, texture);

	shared_ptr<Entity> entity = make_shared<Entity>(textured_model);
	m_scene.addToScene(entity);
}

void Application::destroy() {
	m_shader.destroy();
	m_loader.destroy();
}