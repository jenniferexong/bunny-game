
#include "Entity.h"

void Entity::move(float dx, float dy, float dz)
{
	position_ += vec3(dx, dy, dz);
}

void Entity::rotate(float yaw, float pitch, float roll)
{
	rotation_ += vec3(yaw, pitch, roll);
}
