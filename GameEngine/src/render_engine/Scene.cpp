
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"

#include "../Player.h"
#include "../Entity.h"

using namespace glm;
using namespace std;

/**
	Render all the objects in the scene, updating the projection and view matrices
*/
void Scene::render(GLFWwindow *window, shared_ptr<Player> player) {

	// Getting the width and height of window (may need to update perspective)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height); 
	glViewport(0, 0, width, height); // set viewport to draw to the entire window

	// TODO: May need to change
	mat4 proj = perspective(1.f, (float)width / height, 0.1f, 200.f);
	mat4 view = translate(mat4(1.f), -player->getPosition());
	
	// clear
	glClearColor(0.f, 0.f, 0.f, 0.f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// iterate through all the entities and render them 
	for (shared_ptr<Entity> e : m_entities) {
		e->render();
	}
}

void Scene::addToScene(shared_ptr<Entity> entity) {
	m_entities.push_back(entity);
}
