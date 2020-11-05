
#include "Player.h"

#include "../Application.h"

const float Player::run_speed = 30.f; // per second
const float Player::turn_speed = 400.f; // degrees per second
const float Player::gravity = -50.f;
const float Player::jump_power = 30.f;
const float Player::terrain_height = 0.f;

void Player::updatePosition()
{
	updateSpeed();
	rotate(current_turn_speed_ * Application::frame_delta, 0, 0);
	float forward_distance = forward_speed_ * Application::frame_delta;
	float side_distance = side_speed_ * Application::frame_delta;

	// calculating new position (forward movement)
	float fx = forward_distance * glm::sin(glm::radians(rotation_.x));
	float fz = forward_distance * glm::cos(glm::radians(rotation_.x));
	// side movement
	float sx = side_distance * glm::cos(glm::radians(-rotation_.x));
	float sz = side_distance * glm::sin(glm::radians(-rotation_.x));

	// calculating new y position
	float y = up_velocity_ * Application::frame_delta;

	move(fx + sx, y, fz + sz);

	// Check if hits the ground
	if (position_.y < terrain_height) {
		position_.y = terrain_height;
		up_velocity_ = 0;
	}

	current_turn_speed_ = 0;
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

void Player::changeDirection(double amount)
{
	if (amount > 0)
		current_turn_speed_ = -turn_speed;
	else if (amount < 0)
		current_turn_speed_ = turn_speed;
}


void Player::updateSpeed()
{
	// movement forward and backwards
	if (Application::move_keys[Application::Key::W]) {
		// make the player face forwards
		rotation_offset_ = default_rotation_offset_;
		forward_speed_ = -run_speed;
	} else if (Application::move_keys[Application::Key::S]) {
		// make the player face backwards
		rotation_offset_ = default_rotation_offset_ + vec3(180.f, 0, 0);
		forward_speed_ = run_speed;
	} else {
		forward_speed_ = 0;
	}

	// jumping
	if (Application::move_keys[Application::Key::Space])
		jump();

	fall();

	// Running to the right
	if (Application::move_keys[Application::Key::D]) {
		// make the player face to the right
		rotation_offset_ = default_rotation_offset_ - vec3(90.f, 0, 0); 
		side_speed_ = run_speed;
	} else if (Application::move_keys[Application::Key::A]) { // running left
		// make the player face to the left
		rotation_offset_ = default_rotation_offset_ + vec3(90.f, 0 , 0);
		side_speed_ = -run_speed;
	} else {
		side_speed_ = 0;
	}
}



