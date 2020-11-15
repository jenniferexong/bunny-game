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


