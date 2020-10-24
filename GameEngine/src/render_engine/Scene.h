#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Entity;
class Light;
class Player;

class Scene {
private:
	std::vector<std::shared_ptr<Entity>> m_entities;
	std::shared_ptr<Light> m_global_light;

public:
	void render(GLFWwindow* window, std::shared_ptr<Player> player);
	void addToScene(std::shared_ptr<Entity> entity);
};
