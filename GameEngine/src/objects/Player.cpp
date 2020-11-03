
#include "Player.h"

#include "../Application.h"

const float Player::run_speed = 20.f; // per second
const float Player::turn_speed = 160.f; // degrees per second
const float Player::gravity = -50.f;
const float Player::jump_power = 30.f;
const float Player::terrain_height = 0.f;

void Player::updatePosition()
{
	updateSpeed();
	rotate(_current_turn_speed * Application::frame_delta, 0, 0);
	float distance = _current_speed * Application::frame_delta;

	float offset_angle = -90.f;
	// calculating new position (x, z)
	float x = distance * glm::sin(glm::radians(_rotation.x + offset_angle));
	float z = distance * glm::cos(glm::radians(_rotation.x + offset_angle));

	// calculating new y position
	float y = _up_velocity * Application::frame_delta;

	move(x, y, z);

	// Check if hits the ground
	if (_position.y < terrain_height) {
		_position.y = terrain_height;
		_up_velocity = 0;
	}
}

void Player::jump()
{
	if (_position.y == terrain_height)
		_up_velocity = jump_power;
}

void Player::fall()
{
	_up_velocity += gravity * Application::frame_delta;
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

	// jumping
	if (Application::move_keys[Application::Key::Space])
		jump();

	fall();

	// Turning clockwise
	if (Application::move_keys[Application::Key::D]) {
		_current_turn_speed = -turn_speed;
	} else if (Application::move_keys[Application::Key::A]) { // Turning anticlockwise
		_current_turn_speed = turn_speed;
	} else {
		_current_turn_speed = 0;
	}

}


