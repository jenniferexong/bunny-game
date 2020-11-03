#pragma once

#include <glm/glm.hpp>

using glm::vec3;

class Camera {
private:
	static const float s_move_offset;
	vec3 m_position;
	vec3 m_rotation; // yaw, pitch, roll

public:
	Camera() : m_position(vec3(0, 5, 0)), m_rotation(vec3(0)) {}

	void updatePosition();

	vec3 getPosition() const { return m_position; }
	vec3 getRotation() const { return m_rotation; }
};

