#pragma once

#include <glm/glm.hpp>

using namespace glm;

/* Player (first person view) */
class Player {
private:
	static const vec3 s_size; // width, height, depth

	vec3 m_position; // position of the middle of the feet
	float m_velocity = 1.f;
	vec3 m_direction{ 0.f, 0.f, -1.f }; // direction the player is looking

public:
	Player(vec3 pos) : m_position(pos) {}

	void move(float offset);
	inline vec3 getPosition() { return m_position; }

	vec3 getEyePosition(); // returns the position of the player's eye (this will be the camera)
};
