
#pragma once

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 position_;
	glm::vec3 color_;

public:
	static const int max_lights;
	Light(glm::vec3 position, glm::vec3 color) : position_(position), color_(color) {}

	void setPosition(glm::vec3 position) { position_ = position; }
	void setColor(glm::vec3 color) { color_ = color; }

	glm::vec3 getPosition() const { return position_; }
	glm::vec3 getColor() const { return color_; }
};