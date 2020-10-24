
#include "Player.h"

const vec3 Player::s_size = vec3(0.6f, 1.6f, 0.2f);

/**
	Move the player by a certain amount in the direction player is facing
*/
void Player::move(float offset) {
	// TODO: Project m_direction onto the ground first?
	m_position += m_velocity * normalize(m_direction);
}

vec3 Player::getEyePosition() {
	vec3 eye = m_position; // middle of bottom of feet
	eye += vec3(0.f, 0.8 * s_size.y, 0.f); // offset by some amount of player's height
	return eye;
}


