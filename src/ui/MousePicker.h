﻿#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "RayIntersection.h"

class Entity;
class Environment;
class Camera;

class MousePicker {
private:
	glm::vec3 current_ray_origin_, current_ray_direction_;
	glm::mat4 projection_matrix_, view_matrix_;

	void calculateMouseRay();
	RayIntersection getIntersection(std::weak_ptr<Entity> entity);
	
public:
	~MousePicker() = default;
	MousePicker(): 
		current_ray_origin_(glm::vec3(0)), 
		current_ray_direction_(glm::vec3(0)),
		projection_matrix_(glm::mat4(1)),
		view_matrix_(glm::mat4(1)) {}

	// update the projection and view matrix
	void update(glm::mat4 projection_matrix, const Camera& camera); 
	std::weak_ptr<Entity> selectEntity(Environment& environment);

	glm::vec3 getCurrentRay() { return current_ray_direction_; }
};
