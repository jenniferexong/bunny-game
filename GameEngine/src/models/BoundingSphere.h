#pragma once
#include <glm/vec4.hpp>

class BoundingSphere {
private:
	glm::vec4 model_center_;
	glm::vec4 model_radius_;

public:
	BoundingSphere() : model_center_(glm::vec4(0.f)), model_radius_(glm::vec4(0.f)) {}
	BoundingSphere(glm::vec4 center, glm::vec4 radius) : model_center_(center), model_radius_(radius) {}

	glm::vec4 getCenter() const { return model_center_; }
	glm::vec4 getRadius() const { return model_radius_; }
};
