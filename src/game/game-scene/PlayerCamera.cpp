#include "PlayerCamera.h"

#include "Player.h"
#include "../../engine/environment/Water.h"
#include "../../engine/environment/Terrain.h"

const float PlayerCamera::min_distance = 5;
const float PlayerCamera::max_distance = 30;

float PlayerCamera::distance_from_player = 12;

using namespace glm;

/* Positive amount = zoom in */
void PlayerCamera::zoom(float amount, const vector<Water>& water)
{
	if (
		player_->isInWater(water) 
		&& position_.y <= Water::height + 1.f 
		&& amount > 0
	)
		return;

	distance_from_player -= amount;
	distance_from_player = clamp(
		distance_from_player, min_distance, max_distance
	);
}

void PlayerCamera::changePitch(float amount)
{
	rotation_.y += amount;
	rotation_.y = clamp(rotation_.y, -90.f, 90.f);
}

/* calculate the position of the camera based on the player */
void PlayerCamera::updateView(const Terrain& terrain, const vector<Water>& water) {

	vec3 player_position = player_->getPosition();
	vec3 player_rotation = player_->getRotation();

	// get the direction the player is looking in
	float dx = 1.f * glm::sin(glm::radians(player_rotation.x));
	float dz = 1.f * glm::cos(glm::radians(player_rotation.x));

	float view_angle = rotation_.y;
	float distance_behind_player = (
		distance_from_player * cos(radians(view_angle))
	);
	float height_above_player = (
		distance_from_player 
		* sin(radians(view_angle)) 
		+ (0.4f * distance_from_player)
	);

	vec3 offset = distance_behind_player * vec3(dx, 0, dz);
	position_ = player_position + offset;
	// set height of camera
	position_.y = player_position.y + height_above_player; 

	float min_height = (
		terrain.getHeightOfTerrain(position_.x, position_.z) + 3.f
	);
	position_.y = max(position_.y, min_height);

	// stop camera from going under the water
	if (player_->isInWater(water))
		position_.y = max(position_.y, Water::height + 1.f);

	rotation_.x = -player_rotation.x;

	// update the view frustum
    Camera::updateView();
}

/* Moves camera under water, then inverts the pitch */
void PlayerCamera::positionForReflection(float water_height)
{
	float distance = position_.y - water_height;
	position_.y -= 2 * distance;
	rotation_.y *= -1;
}

/* Moves camera back to original position above water */
void PlayerCamera::positionForRefraction(float water_height)
{
	float distance = water_height - position_.y;
	position_.y += 2 * distance;
	rotation_.y *= -1; // invert
}
