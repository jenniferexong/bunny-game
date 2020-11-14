#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "RayIntersection.h"

#include "../models/Model.h"
#include "../environment/Entity.h"

class Environment;
class Camera;

class MousePicker {
private:
	glm::vec3 current_ray_origin_, current_ray_direction_;
	glm::mat4 projection_matrix_, view_matrix_;

	void calculateMouseRay();
	RayIntersection getIntersection(const std::shared_ptr<Entity>& entity);
	
public:
	MousePicker() : current_ray_direction_(glm::vec3(0.f)), projection_matrix_(glm::mat4(1.f)), view_matrix_(glm::mat4(1.f)) {}

	void update(glm::mat4 projection_matrix, const Camera& camera); // update the projection and view matrix
	std::shared_ptr<Entity> selectEntity(Environment& environment);

	glm::vec3 getCurrentRay() { return current_ray_direction_; }
};
