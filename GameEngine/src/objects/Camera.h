#pragma once

#include <glm/glm.hpp>

using glm::vec3;

class Camera {
private:
	static const float move_offset;
	vec3 position_;
	vec3 rotation_; // yaw, pitch, roll

public:
	Camera() : position_(vec3(150.f, 5, 0)), rotation_(vec3(0)) {}

	void updatePosition();

	vec3 getPosition() const { return position_; }
	vec3 getRotation() const { return rotation_; }
};

