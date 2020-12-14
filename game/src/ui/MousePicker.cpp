#include "MousePicker.h"

#include "../game-manager/Application.h"
#include "../pkg/Environment.h"
#include "../util/Maths.h"

void MousePicker::update(glm::mat4 projection_matrix, const Camera& camera)
{
	projection_matrix_ = projection_matrix;
	view_matrix_ = Maths::createViewMatrix(camera);
	calculateMouseRay();
}

std::weak_ptr<Entity> MousePicker::selectEntity(Environment& environment)
{
	weak_ptr<Entity> selected;
	float max_distance = 1000.f;
	float min_distance = std::numeric_limits<float>::max();

	// iterate through all the entities and check for ray intersection with its bounding sphere
	for (const auto& element: environment.getEntitiesInView()) {
		for (const auto& e: element.second) {
			RayIntersection intersect = getIntersection(e);

			// check if it is the closest intersection
			if (intersect.valid_intersection) {
				float distance = glm::length(current_ray_origin_ 
						- intersect.intersection_point);
				if (e.lock()->isSelectable() && distance < min_distance 
						&& distance < max_distance) {
					selected = e;
				}
			}	
		}
	}
	return selected;
}

using namespace glm;
RayIntersection MousePicker::getIntersection(weak_ptr<Entity> entity)
{
	glm::mat4 model_matrix = Maths::createTransformationMatrix(
		entity.lock()->getPosition(), 
		entity.lock()->getActualRotation(),
		entity.lock()->getScale(), 
		entity.lock()->getAlignmentRotation()
	);

	const auto& bounding_sphere = 
		entity.lock()->getModel().getMesh().getBoundingSphere();
	vec3 sphere_center = vec3(model_matrix * bounding_sphere.getCenter());
	float sphere_radius = 
		entity.lock()->getScale() * bounding_sphere.getRadius();

	// ray intersection with sphere
	float a, b, c;
	a = dot(current_ray_direction_, current_ray_direction_);
	b = 2 * (dot(current_ray_origin_ - sphere_center, current_ray_direction_));
	c = dot(
		current_ray_origin_ - sphere_center,
		current_ray_origin_ - sphere_center) - (sphere_radius * sphere_radius
	);

	float min_t = std::numeric_limits<float>::max();
	float discriminant = (b * b) - (4 * a * c);

	// no solutions
	if (discriminant < 0)
		return {};

	float t = (-b + sqrt(discriminant)) / (2 * a);
	if (t < min_t && t > 0)
		min_t = t;

	t = (-b - sqrt(discriminant)) / (2 * a);
	if (t < min_t && t > 0)
		min_t = t;

	if (min_t >= std::numeric_limits<float>::max()) // no valid intersection
		return {};

	// calculate point of intersection 
	vec3 intersection = current_ray_origin_ + (min_t * current_ray_direction_);
	return { true, intersection };
}

using namespace glm;

/**
 * Calculates the projected mouse direction based on 
 * its 2d position on the screen
 */
void MousePicker::calculateMouseRay()
{
	// viewport space coordinates

	// normalized devices coordinates
	vec2 normalized_coords(0.f, 0);

	// homogeneous clip space
	vec4 clip_coords(normalized_coords.x, normalized_coords.y, -1.f, 1);

	// eye space
	vec4 eye_coords(inverse(projection_matrix_) * clip_coords);
	eye_coords.z = -1.f;
	eye_coords.w = 1.f;

	// world space
	vec4 world_coords(inverse(view_matrix_) * eye_coords);
	current_ray_origin_ = vec3(world_coords);

	eye_coords.w = 0.f;
	vec4 world_direction(inverse(view_matrix_) * eye_coords);
	current_ray_direction_ = normalize(vec3(world_direction));
} 
