#include "Camera.h"

/* Moves camera under water, then inverts the pitch */
void Camera::positionForReflection(float water_height)
{
	float distance = position_.y - water_height;
	position_.y -= 2 * distance;
	rotation_.y *= -1;
}

/* Moves camera back to original position above water */
void Camera::positionForRefraction(float water_height)
{
	float distance = water_height - position_.y;
	position_.y += 2 * distance;
	rotation_.y *= -1; // invert
}
