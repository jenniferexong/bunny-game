
#include "Entity.h"

void Entity::move(float dx, float dy, float dz)
{
	m_position += vec3(dx, dy, dz);
}

void Entity::rotate(float yaw, float pitch, float roll)
{
	m_rotation += vec3(yaw, pitch, roll);
}
