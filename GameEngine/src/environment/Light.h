
#pragma once

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 position_;
	glm::vec3 color_;
	glm::vec3 attenuation_ = glm::vec3(1.f, 0, 0);

public:
	static const int max_lights;
	static const glm::vec3 point_light_attenuation;
	
	Light(glm::vec3 position, glm::vec3 color) : position_(position), color_(color) {}
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation) : position_(position), color_(color), attenuation_(attenuation) {}

	void setPosition(glm::vec3 position) { position_ = position; }
	void setColor(glm::vec3 color) { color_ = color; }

	glm::vec3 getPosition() const { return position_; }
	glm::vec3 getColor() const { return color_; }
	glm::vec3 getAttenuation() const { return attenuation_; }
};