
#include "Player.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

const float Player::run_speed = 30.f; // per second
const float Player::turn_speed = 100.f; // degrees per second
const float Player::gravity = -70.f;
const float Player::jump_power = 40.f;

using namespace glm;

vec3 Player::getRotationOffset()
{
	switch (state_) {
	case DirectionState::N:
		return default_rotation_offset_;
	case DirectionState::S:
		return default_rotation_offset_ + vec3(180.f, 0, 0);
	case DirectionState::E:
		return default_rotation_offset_ + vec3(-90.f, 0, 0);
	case DirectionState::W:
		return default_rotation_offset_ + vec3(90.f, 0, 0);
	case DirectionState::NE:
		return default_rotation_offset_ + vec3(-45.f, 0, 0);
	case DirectionState::SE:
		return default_rotation_offset_ + vec3(-135.f, 0, 0);
	case DirectionState::SW:
		return default_rotation_offset_ + vec3(135.f, 0, 0);
	case DirectionState::NW:
		return default_rotation_offset_ + vec3(45.f, 0, 0);
	default:
		return vec3(0);
	}
}

void Player::updatePosition(const Terrain& terrain)
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

	float terrain_height = terrain.getHeightOfTerrain(position_.x, position_.z);
	rotation_offset_ = getRotationOffset();

	// Check if hits the ground
	if (position_.y < terrain_height) {
		position_.y = terrain_height;
		is_in_air = false;
		up_velocity_ = 0;

		// Align with the normal of the terrain at current position
		setAlignmentRotation(terrain.getNormalOfTerrain(position_.x, position_.z));
	}
	current_turn_speed_ = 0;
}

void Player::jump()
{
	if (!is_in_air) {
		up_velocity_ = jump_power;
		is_in_air = true;
	}
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
	forward_speed_ = 0;
	side_speed_ = 0;
	if (Application::move_keys[Application::Key::W]) {
		if (Application::move_keys[Application::Key::A]) { // NW
			state_ = DirectionState::NW;
			forward_speed_ = -run_speed;
			side_speed_ = -run_speed;
		}
		else if (Application::move_keys[Application::Key::D]) { // NE
			state_ = DirectionState::NE;
			forward_speed_ = -run_speed;
			side_speed_ = run_speed;
		}
		else { // N
			state_ = DirectionState::N;
			forward_speed_ = -run_speed;
		}
	}
	else if (Application::move_keys[Application::Key::S]) {
		if (Application::move_keys[Application::Key::A]) { // SW
			state_ = DirectionState::SW;
			forward_speed_ = run_speed;
			side_speed_ = -run_speed;
		}
		else if (Application::move_keys[Application::Key::D]) { // SE
			state_ = DirectionState::SE;
			forward_speed_ = run_speed;
			side_speed_ = run_speed;
		}
		else { // S
			state_ = DirectionState::S;
			forward_speed_ = run_speed;
		}
			
	}
	else if (Application::move_keys[Application::Key::D]) { // E
		// make the player face to the right
		state_ = DirectionState::E;
		side_speed_ = run_speed;
	}
	else if (Application::move_keys[Application::Key::A]) { // W
		// make the player face to the left
		state_ = DirectionState::W;
		side_speed_ = -run_speed;
	}

	// jumping
	if (Application::move_keys[Application::Key::Space])
		jump();

	fall();
}



