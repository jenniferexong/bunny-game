
#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

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
	const vec3 up = vec3(0, 1.f, 0);
	if (surface_normal != up) {
		float angle_between = -acos(dot(surface_normal, up));
		vec3 rotation_axis = cross(surface_normal, up);
		glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1), angle_between, rotation_axis);
		alignment_rotation_ = rotation_matrix;
	}
}

