#include "Camera.h"

#include <glm/gtx/projection.hpp>

#include "../Application.h"

const float Camera::min_distance = 5;
const float Camera::max_distance = 30;

float Camera::distance_from_player = 12;

using namespace glm;

void Camera::zoom(float amount)
{
	distance_from_player -= amount;
	distance_from_player = clamp(distance_from_player, min_distance, max_distance);
}

void Camera::changePitch(float amount)
{
	rotation_.y += amount;
	rotation_.y = clamp(rotation_.y, -90.f, 90.f);
}

/* calculate the position of the camera based on the player */
void Camera::updateView(const Terrain& terrain) {

	vec3 player_position = player_->getPosition();
	vec3 player_rotation = player_->getRotation();

	// get the direction the player is looking in
	float dx = 1.f * glm::sin(glm::radians(player_rotation.x));
	float dz = 1.f * glm::cos(glm::radians(player_rotation.x));

	float view_angle = rotation_.y;
	float distance_behind_player = distance_from_player * cos(radians(view_angle));
	float height_above_player = distance_from_player * sin(radians(view_angle)) + (0.4f * distance_from_player);

	vec3 offset = distance_behind_player * vec3(dx, 0, dz);
	position_ = player_position + offset;
	position_.y = player_position.y + height_above_player; // set height of camera

	float min_height = terrain.getHeightOfTerrain(position_.x, position_.z) + 3.f;
	position_.y = max(position_.y, min_height);

	rotation_.x = -player_rotation.x;
}

void Camera::print()
{
	printf("Camera: x: %.1f, y: %.1f, z: %.1f, y: %.1f, p: %.1f, r: %.1f\n",
		position_.x, position_.y, position_.z, rotation_.x, rotation_.y, rotation_.z);
}