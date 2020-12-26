
#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../util/Maths.h"
#include "../util/Log.h"

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
	brightness_ = 0.f;
}

bool Entity::collides(const Entity& other)
{
	if (id_ == other.id_)
		return false;

	float distance = glm::distance(getCenterWorld(), other.getCenterWorld());
	return distance < getRadius() + other.getRadius();
}

glm::vec3 Entity::getCenterWorld() const 
{
	return vec3(
		getModelMatrix() * model_.getMesh().getBoundingSphere().getCenter());
}

float Entity::getRadius() const
{
   return scale_ * model_.getMesh().getBoundingSphere().getRadius();
}

glm::mat4 Entity::getModelMatrix() const
{
	return Maths::createTransformationMatrix(
		position_,
		getActualRotation(),
		scale_,
		alignment_rotation_
	);
}

void Entity::print() {
	printf(
		"entity: %d, texture: %d, vao: %d, vertex count: %d\n",
		id_,
		model_.getTexture().getTextureId(), 
		model_.getMesh().getId(), 
		model_.getMesh().getVertexCount()
	);
}
