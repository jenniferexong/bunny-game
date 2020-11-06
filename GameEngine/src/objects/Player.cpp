
#include "Player.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "../Application.h"

const float Player::run_speed = 30.f; // per second
const float Player::turn_speed = 100.f; // degrees per second
const float Player::gravity = -70.f;
const float Player::jump_power = 40.f;

using std::cout;
using std::endl;
using namespace glm;

vec3 Player::getRotationOffset()
{
	switch (state_) {
	case State::N:
		return default_rotation_offset_;
	case State::S:
		return default_rotation_offset_ + vec3(180.f, 0, 0);
	case State::E:
		return default_rotation_offset_ + vec3(-90.f, 0, 0);
	case State::W:
		return default_rotation_offset_ + vec3(90.f, 0, 0);
	case State::NE:
		return default_rotation_offset_ + vec3(-45.f, 0, 0);
	case State::SE:
		return default_rotation_offset_ + vec3(-135.f, 0, 0);
	case State::SW:
		return default_rotation_offset_ + vec3(135.f, 0, 0);
	case State::NW:
		return default_rotation_offset_ + vec3(45.f, 0, 0);
	default:
		return vec3(0);
	}
}

void Player::updatePosition(const Terrain& terrain)
{
	updateSpeed();
	rotate(current_turn_speed_ * Application::frame_delta, 0, 0);
	//mat4 rotate_up_vector = glm::rotate(mat4(1), radians(-rotation_.x), vec3(0, 1.f, 0));
	//up_vector_ = vec3(rotate_up_vector * vec4(up_vector_, 0)); // update up vector

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

		/*
		// calculate the rotation matrix needed to align player up vector with the normal of the terrain location 
		vec3 surface_normal = terrain.getNormalOfTerrain(position_.x, position_.z);
		float roll = -atan2(sqrt(surface_normal.y * surface_normal.y + surface_normal.z * surface_normal.z), surface_normal.x);
		float pitch = -atan2(surface_normal.y, surface_normal.z);
		float pitch = -(atan2(surface_normal.y, sqrt((surface_normal.x * surface_normal.x) + (surface_normal.z * surface_normal.z))) + (pi<float>() / 2));
		float roll = -(atan2(surface_normal.z, surface_normal.x) + (pi<float>() / 2));
		rotation_offset_.y += degrees(pitch);
		rotation_offset_.z += degrees(roll);
		*/

		/*
		float angle_between = -acos(dot(surface_normal, up_vector_));
		vec3 rotation_axis = cross(surface_normal, up_vector_);
		mat4 rotation_matrix = glm::rotate(mat4(1), angle_between, rotation_axis);
		alignment_rotation_ = rotation_matrix;
		up_vector_ = surface_normal;
		*/
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
			state_ = State::NW;
			forward_speed_ = -run_speed;
			side_speed_ = -run_speed;
		}
		else if (Application::move_keys[Application::Key::D]) { // NE
			state_ = State::NE;
			forward_speed_ = -run_speed;
			side_speed_ = run_speed;
		}
		else { // N
			state_ = State::N;
			forward_speed_ = -run_speed;
			//rotation_offset_ += vec3(0.f, 0, 0);
		}
	}
	else if (Application::move_keys[Application::Key::S]) {
		if (Application::move_keys[Application::Key::A]) { // SW
			state_ = State::SW;
			forward_speed_ = run_speed;
			side_speed_ = -run_speed;
		}
		else if (Application::move_keys[Application::Key::D]) { // SE
			state_ = State::SE;
			forward_speed_ = run_speed;
			side_speed_ = run_speed;
		}
		else { // S
			state_ = State::S;
			//rotation_offset_ += vec3(180.f, 0, 0);
			forward_speed_ = run_speed;
		}
			
	}
	else if (Application::move_keys[Application::Key::D]) { // E
		// make the player face to the right
		state_ = State::E;
		//rotation_offset_ += -vec3(90.f, 0, 0); 
		side_speed_ = run_speed;
	}
	else if (Application::move_keys[Application::Key::A]) { // W
		// make the player face to the left
		state_ = State::W;
		//rotation_offset_ += vec3(90.f, 0, 0);
		side_speed_ = -run_speed;
	}

	// jumping
	if (Application::move_keys[Application::Key::Space])
		jump();

	fall();

}



