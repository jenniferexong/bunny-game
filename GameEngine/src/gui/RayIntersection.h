#pragma once

#include <glm/vec3.hpp>

struct RayIntersection {

	bool valid_intersection = false;
	glm::vec3 intersection_point = glm::vec3(0);

	RayIntersection() = default; // invalid
	RayIntersection(bool valid, glm::vec3 point) : valid_intersection(valid), intersection_point(point) {}
};
