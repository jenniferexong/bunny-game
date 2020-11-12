#pragma once
#include <glm/glm.hpp>


class MousePicker {
private:
	glm::vec3 current_ray_;
	glm::mat4 projection_matrix_, view_matrix_;

	glm::vec3 calculateMouseRay();
	
public:
	MousePicker() : current_ray_(glm::vec3(0.f)), projection_matrix_(glm::mat4(1.f)), view_matrix_(glm::mat4(1.f)) {}

	void update(); // update the projection and view matrix
	glm::vec3 getCurrentRay() { return current_ray_; }
};
