#include "Camera.h"
#include "../Application.h"

const float Camera::height_above_player = 10;
const float Camera::distance_behind_player = 15;

using namespace glm;

void Camera::updateView() {
	// calculate the position of the camera based on the player
	vec3 player_position = player_->getPosition();

	vec3 player_rotation = player_->getRotation();

	// get the direction the player is looking in
	float dx = 1.f * glm::sin(glm::radians(player_rotation.x));
	float dz = 1.f * glm::cos(glm::radians(player_rotation.x));

	vec3 offset = -distance_behind_player * vec3(dx, 0, dz);
	position_ = player_position - offset;
	position_.y = player_position.y + height_above_player; // set height of camera

	// calulating the pitch of the camera (always the same)
	float angle = degrees(atan(height_above_player/2.f, distance_behind_player));
	rotation_.y = angle;
	rotation_.x = -player_rotation.x;
}

void Camera::print()
{
	printf("Camera: x: %.1f, y: %.1f, z: %.1f, y: %.1f, p: %.1f, r: %.1f\n",
		position_.x, position_.y, position_.z, rotation_.x, rotation_.y, rotation_.z);
}
