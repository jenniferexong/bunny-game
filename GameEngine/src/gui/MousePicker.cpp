#include "MousePicker.h"

#include "../Application.h"

void MousePicker::update()
{
	projection_matrix_ = Application::getProjectionMatrix();
	view_matrix_ = Maths::createViewMatrix(Application::camera);
	current_ray_ = calculateMouseRay();

	//printf("%.2f, %.2f, %.2f\n", current_ray_.x, current_ray_.y, current_ray_.z);
}

using namespace glm;

/* Calculates the projected mouse direction based on its 2d position on the screen */
glm::vec3 MousePicker::calculateMouseRay()
{
	// viewport space coordinates

	// normalized devices coordinates
	vec2 normalized_coords(0.f, 0);

	// homogeneous clip space
	vec4 clip_coords(normalized_coords.x, normalized_coords.y, -1.f, 1);

	// eye space
	vec4 eye_coords(inverse(projection_matrix_) * clip_coords);
	eye_coords.z = -1.f;
	eye_coords.w = 0.f;

	// world space
	vec4 world_coords(inverse(view_matrix_) * eye_coords);

	vec3 mouse_ray = normalize(vec3(world_coords));
	return mouse_ray;
} 
