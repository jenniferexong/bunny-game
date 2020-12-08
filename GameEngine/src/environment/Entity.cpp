
#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"
#include "../Maths.h"

int Entity::next_id_ = 0;

void Entity::move(float dx, float dy, float dz)
{
	position_ += vec3(dx, dy, dz);
}

void Entity::rotate(float yaw, float pitch, float roll)
{
	rotation_ += vec3(yaw, pitch, roll);
}

void Entity::setRotationOffset(float yaw, float pitch, float roll)
{
	default_rotation_offset_ = vec3(yaw, pitch, roll);
	rotation_offset_ = default_rotation_offset_;
}

void Entity::setAlignmentRotation(vec3 surface_normal)
{
	alignment_rotation_ = Maths::getAlignmentRotation(surface_normal);
}

void Entity::highlight()
{
	brightness_ = 3.f;
}

void Entity::unhighlight()
{
	brightness_ = 0.8f;
}
