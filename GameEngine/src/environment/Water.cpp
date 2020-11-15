#include "Water.h"

const float Water::tile_size = 50.f;

glm::vec4 Water::getReflectionPlane()
{
	return { 0, 1.f, 0, 15 };
}

glm::vec4 Water::getRefractionPlane()
{
	return { 0, -1.f, 0, -15 };
}

/* Checks if the point is under this water tile */
bool Water::containsPoint(glm::vec3 point) const 
{
	float min_x = centre_x - tile_size;
	float max_x = centre_x + tile_size;
	float min_z = centre_z - tile_size;
	float max_z = centre_z + tile_size;

	return point.x < max_x && point.x > min_x && point.z < max_z && point.z > min_z && point.y <= height_;
}



