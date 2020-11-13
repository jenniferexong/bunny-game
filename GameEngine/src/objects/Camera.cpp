#include "Camera.h"
#include "../Application.h"

const float Camera::min_distance = 10;
const float Camera::max_distance = 30;

float Camera::pitch = 1.f;
float Camera::distance_from_player = 12;

using namespace glm;

void Camera::zoom(float amount)
{
	distance_from_player -= amount;
	distance_from_player = clamp(distance_from_player, min_distance, max_distance);
}

void Camera::changePitch(double amount)
{
	pitch += float(amount);
	pitch = clamp(pitch, -90.f, 90.f);
}

void Camera::updateView() {
	// calculate the position of the camera based on the player
	vec3 player_position = player_->getPosition();

	vec3 player_rotation = player_->getRotation();

	// get the direction the player is looking in
	float dx = 1.f * glm::sin(glm::radians(player_rotation.x));
	float dz = 1.f * glm::cos(glm::radians(player_rotation.x));

	float view_angle = glm::clamp(pitch, 0.1f, 90.f);
	float distance_behind_player = distance_from_player * cos(radians(view_angle));
	float height_above_player = distance_from_player * sin(radians(view_angle)) + (0.4f * distance_from_player);

	vec3 offset = distance_behind_player * vec3(dx, 0, dz);
	position_ = player_position + offset;
	position_.y = player_position.y + height_above_player; // set height of camera

	rotation_.y = pitch;

	rotation_.x = -player_rotation.x;
}

void Camera::print()
{
	printf("Camera: x: %.1f, y: %.1f, z: %.1f, y: %.1f, p: %.1f, r: %.1f\n",
		position_.x, position_.y, position_.z, rotation_.x, rotation_.y, rotation_.z);
}
