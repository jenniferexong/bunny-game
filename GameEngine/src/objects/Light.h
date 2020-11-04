
#pragma once

#include <glm/glm.hpp>

class Light {
private:
	vec3 position_;
	vec3 color_;

public:
	Light(vec3 position, vec3 color) : position_(position), color_(color) {}

	void setPosition(vec3 position) { position_ = position; }
	void setColor(vec3 color) { color_ = color; }

	vec3 getPosition() const { return position_; }
	vec3 getColor() const { return color_; }
};