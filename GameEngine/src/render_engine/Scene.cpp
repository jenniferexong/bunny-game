
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"

#include "../object/Player.h"
#include "../object/Entity.h"

using namespace glm;
using namespace std;

/**
	Render all the objects in the scene, updating the projection and view matrices
*/
void Scene::render(GLFWwindow *window, shared_ptr<Player> player)
{
}

void Scene::addToScene(shared_ptr<Entity> entity) {
	m_entities.push_back(entity);
}
