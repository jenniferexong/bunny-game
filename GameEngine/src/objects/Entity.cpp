
#include "Entity.h"

void Entity::move(float dx, float dy, float dz)
{
	_position += vec3(dx, dy, dz);
}

void Entity::rotate(float yaw, float pitch, float roll)
{
	_rotation += vec3(yaw, pitch, roll);
}
