
#include "Player.h"

#include "../Application.h"

const float Player::run_speed = 20.f; // per second
const float Player::turn_speed = 160.f; // degrees per second

void Player::updatePosition()
{
	updateSpeed();
	rotate(_current_turn_speed * Application::frame_delta, 0, 0);
	float distance = _current_speed * Application::frame_delta;

	// calculating new position
	float x = distance * glm::sin(glm::radians(_rotation.x));
	float z = distance * glm::cos(glm::radians(_rotation.x));
	move(x, 0, z);
}

void Player::updateSpeed()
{
	// movement forward and backwards
	if (Application::move_keys[Application::Key::W]) {
		_current_speed = run_speed;
	} else if (Application::move_keys[Application::Key::S]) {
		_current_speed = -run_speed;
	} else {
		_current_speed = 0;
	}

	// Turning clockwise
	if (Application::move_keys[Application::Key::D]) {
		_current_turn_speed = -turn_speed;
	} else if (Application::move_keys[Application::Key::A]) { // Turning anticlockwise
		_current_turn_speed = turn_speed;
	} else {
		_current_turn_speed = 0;
	}

}


