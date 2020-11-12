#pragma once

#include <glm/glm.hpp>

#include <map>
#include <memory>
#include <set>

#include "RayIntersection.h"

#include "../CompareTexturedModel.h"
#include "../models/Model.h"
#include "../objects/Entity.h"


class MousePicker {
private:
	glm::vec3 current_ray_origin_, current_ray_direction_;
	glm::mat4 projection_matrix_, view_matrix_;

	void calculateMouseRay();
	RayIntersection getIntersection(const std::shared_ptr<Entity>& entity);
	
public:
	MousePicker() : current_ray_direction_(glm::vec3(0.f)), projection_matrix_(glm::mat4(1.f)), view_matrix_(glm::mat4(1.f)) {}

	void update(); // update the projection and view matrix
	std::shared_ptr<Entity> selectEntity(const std::map<std::shared_ptr<TexturedModel>, std::shared_ptr<std::set<std::shared_ptr<Entity>>>, CompareTexturedModel>& entities);

	glm::vec3 getCurrentRay() { return current_ray_direction_; }
};
