#pragma once

#include <glm/glm.hpp>

#include <map>

using glm::vec3;

class Camera {
private:
	static const float s_move_offset;
	vec3 m_position;
	vec3 m_rotation; // yaw, pitch, roll

public:

	Camera() : m_position(vec3(0)), m_rotation(vec3(0)) {}

	void updatePosition();

	inline vec3 getPosition() const { return m_position; }
	inline vec3 getRotation() const { return m_rotation; }
};

