
#include "Player.h"

#include "../Application.h"

const float Player::run_speed = 30.f; // per second
const float Player::turn_speed = 130.f; // degrees per second
const float Player::gravity = -50.f;
const float Player::jump_power = 30.f;
const float Player::terrain_height = 0.f;

void Player::updatePosition()
{
	updateSpeed();
	rotate(current_turn_speed_ * Application::frame_delta, 0, 0);
	float distance = current_speed_ * Application::frame_delta;

	//float offset_angle = -90.f;
	// calculating new position (x, z)
	float x = distance * glm::sin(glm::radians(rotation_.x));
	float z = distance * glm::cos(glm::radians(rotation_.x));

	// calculating new y position
	float y = up_velocity_ * Application::frame_delta;

	move(x, y, z);

	// Check if hits the ground
	if (position_.y < terrain_height) {
		position_.y = terrain_height;
		up_velocity_ = 0;
	}
}

void Player::jump()
{
	if (position_.y == terrain_height)
		up_velocity_ = jump_power;
}

void Player::fall()
{
	up_velocity_ += gravity * Application::frame_delta;
}

void Player::updateSpeed()
{
	// movement forward and backwards
	if (Application::move_keys[Application::Key::W]) {
		current_speed_ = -run_speed;
	} else if (Application::move_keys[Application::Key::S]) {
		current_speed_ = run_speed;
	} else {
		current_speed_ = 0;
	}

	// jumping
	if (Application::move_keys[Application::Key::Space])
		jump();

	fall();

	// Turning clockwise
	if (Application::move_keys[Application::Key::D]) {
		current_turn_speed_ = turn_speed;
	} else if (Application::move_keys[Application::Key::A]) { // Turning anticlockwise
		current_turn_speed_ = -turn_speed;
	} else {
		current_turn_speed_ = 0;
	}

}


