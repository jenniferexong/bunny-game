
#include "Player.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

using namespace glm;

Player::Player(TexturedModel model, vec3 position, vec3 rotation, float scale)
		: Entity(model, position, rotation, scale)
{
	selectable_ = false;
}

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

void Player::updatePosition(const Environment& environment, const std::map<Key, bool>& move_keys)
{
	Terrain terrain = environment.getTerrains().at(0);
	updateSpeed(move_keys);

	if (isInWater(environment.getWater())) {
		forward_speed_ /= 3.f;
		side_speed_ /= 3.f;
	}

	float forward_distance = forward_speed_ * app->frame_delta;
	float side_distance = side_speed_ * app->frame_delta;

	// calculating new position (forward movement)
	float fx = forward_distance * glm::sin(glm::radians(rotation_.x));
	float fz = forward_distance * glm::cos(glm::radians(rotation_.x));
	// side movement
	float sx = side_distance * glm::cos(glm::radians(-rotation_.x));
	float sz = side_distance * glm::sin(glm::radians(-rotation_.x));

	// calculating new y position
	float y = up_velocity_ * app->frame_delta;

	move(fx + sx, y, fz + sz);

	float terrain_height = terrain.getHeightOfTerrain(position_.x, position_.z);
	rotation_offset_ = getRotationOffset();

	// swim in water
	if (isInWater(environment.getWater())) {
		float min_height = Water::height -1.75f;
		position_.y = max(min_height, position_.y);

		// floating in water
		if (position_.y <= min_height) {
			setAlignmentRotation(vec3(0, 1.f, 0));
			is_in_air_ = true;
			up_velocity_ = 0;
		}
	}

	// Check if hits the ground
	if (position_.y < terrain_height) {
		position_.y = terrain_height;
		is_in_air_ = false;
		up_velocity_ = 0;

		// Align with the normal of the terrain at current position
		setAlignmentRotation(terrain.getNormalOfTerrain(position_.x, position_.z));
	}
}

bool Player::isInWater(const vector<Water>& water) const
{
	for (const auto& w: water) {
		if (w.containsPoint(position_))
			return true;
	}
	return false;
}


void Player::jump()
{
	if (!is_in_air_) {
		up_velocity_ = jump_power;
		is_in_air_ = true;
	}
}

void Player::fall()
{
	up_velocity_ += gravity * app->frame_delta;
}

void Player::changeDirection(float amount)
{
	rotate(-amount, 0, 0);
}


void Player::updateSpeed(const std::map<Key, bool>& move_keys)
{
	forward_speed_ = 0;
	side_speed_ = 0;
	if (move_keys.at(Key::W)) {
		if (move_keys.at(Key::A)) { // NW
			state_ = DirectionState::NW;
			forward_speed_ = -run_speed;
			side_speed_ = -run_speed;
		}
		else if (move_keys.at(Key::D)) { // NE
			state_ = DirectionState::NE;
			forward_speed_ = -run_speed;
			side_speed_ = run_speed;
		}
		else { // N
			state_ = DirectionState::N;
			forward_speed_ = -run_speed;
		}
	}
	else if (move_keys.at(Key::S)) {
		if (move_keys.at(Key::A)) { // SW
			state_ = DirectionState::SW;
			forward_speed_ = run_speed;
			side_speed_ = -run_speed;
		}
		else if (move_keys.at(Key::D)) { // SE
			state_ = DirectionState::SE;
			forward_speed_ = run_speed;
			side_speed_ = run_speed;
		}
		else { // S
			state_ = DirectionState::S;
			forward_speed_ = run_speed;
		}
			
	}
	else if (move_keys.at(Key::D)) { // E
		// make the player face to the right
		state_ = DirectionState::E;
		side_speed_ = run_speed;
	}
	else if (move_keys.at(Key::A)) { // W
		// make the player face to the left
		state_ = DirectionState::W;
		side_speed_ = -run_speed;
	}

	// jumping
	if (move_keys.at(Key::Space))
		jump();

	fall();
}



