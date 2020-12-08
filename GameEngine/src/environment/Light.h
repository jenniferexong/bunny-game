#pragma once

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 position_ = glm::vec3(0);
	glm::vec3 color_ = glm::vec3(1);
	glm::vec3 attenuation_ = glm::vec3(1.f, 0, 0);

public:
	static constexpr int max_lights = 40;
	static constexpr glm::vec3 point_light_attenuation = glm::vec3(1.f, 0.01, 0.002);
	
	Light() = default;
	~Light() = default;
	Light(glm::vec3 position, glm::vec3 color) : position_(position), color_(color) {}
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation) : position_(position), color_(color), attenuation_(attenuation) {}

	void setPosition(glm::vec3 position) { position_ = position; }
	void setColor(glm::vec3 color) { color_ = color; }

	glm::vec3 getPosition() const { return position_; }
	glm::vec3 getColor() const { return color_; }
	glm::vec3 getAttenuation() const { return attenuation_; }
};