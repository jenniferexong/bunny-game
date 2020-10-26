#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>

class Entity;
class Shader;
class Camera;

class Renderer {
private:
	static const float s_fov;
	static const float s_near_plane;
	static const float s_far_plane;

public:
	static glm::mat4 s_projection_matrix;
	void prepare();
	void render(const Entity& entity, const Shader& shader);
};

