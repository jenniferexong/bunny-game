#include "Water.h"

#include <cmath>

#include "../Application.h"

float Water::move_factor = 0.f;

void Water::updateRipples()
{
	// move the water ripples
	move_factor += wave_speed * app->timer->frame_delta;
	move_factor = fmod(move_factor, 1.f);
}

glm::vec4 Water::getReflectionPlane()
{
	return {0, 1.f, 0, -height};
}

glm::vec4 Water::getRefractionPlane()
{
	float bias = 1.f;
	return {0, -1.f, 0, height + bias};
}

/* Checks if the point is under this water tile */
bool Water::containsPoint(glm::vec3 point) const
{
	float min_x = centre_x - tile_size;
	float max_x = centre_x + tile_size;
	float min_z = centre_z - tile_size;
	float max_z = centre_z + tile_size;

	return (
		point.x < max_x && point.x > min_x && point.z < max_z && point.z > min_z && point.y <= height);
}
