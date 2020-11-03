#pragma once

#include <glm/glm.hpp>

using glm::vec3;

class Camera {
private:
	static const float move_offset;
	vec3 _position;
	vec3 _rotation; // yaw, pitch, roll

public:
	Camera() : _position(vec3(150.f, 5, 0)), _rotation(vec3(0)) {}

	void updatePosition();

	vec3 getPosition() const { return _position; }
	vec3 getRotation() const { return _rotation; }
};

