#include "Maths.h"
#include "../scene/Camera.h"
#include "../game-manager/Application.h"

using namespace glm;

/**
 * Calculates the directional vector based on yaw pitch and roll
 * Note: in the case of the player, this is the direction the player moves in if W is pressed
 */
vec3 Maths::getLookDirection(
	vec3 initial_direction, 
	vec3 rotation, 
	mat4 alignment_matrix)
{
	vec4 direction = vec4(initial_direction, 0);
	float yaw = rotation.x;
	float pitch = rotation.y;
	float roll = rotation.z;
	mat4 matrix = mat4(1);

	matrix = matrix * alignment_matrix;
	matrix = rotate(matrix, radians(yaw), vec3(0, 1, 0));
	matrix = rotate(matrix, radians(pitch), vec3(1, 0, 0));
	matrix = rotate(matrix, radians(roll), vec3(0, 0, 1));
	direction = matrix * direction;
	return vec3(normalize(direction));
}

mat4 Maths::getAlignmentRotation(vec3 surface_normal)
{
	mat4 matrix = mat4(1.f);
	const vec3 up = vec3(0, 1.f, 0);
	if (surface_normal != up) {
		float angle_between = -acos(dot(surface_normal, up));
		vec3 rotation_axis = cross(surface_normal, up);
		matrix = glm::rotate(glm::mat4(1), angle_between, rotation_axis);
	}
	return matrix;
}

/* Creates a transformation matrix for a gui component*/
mat4 Maths::createTransformationMatrix(vec2 t, float r, vec2 s)
{
	mat4 matrix = mat4(1);
	matrix = translate(matrix, vec3(t, 0));
	matrix = scale(matrix, vec3(s, 1));
	matrix = rotate(matrix, radians(r), vec3(0, 0, 1));

	return matrix;
}

/* Helper function: uses Euler angles for rotation */
mat4 Maths::createTransformationMatrix(
	vec3 t, vec3 r, float s, 
	mat4 alignment_matrix)
{
	mat4 matrix = mat4(1);

	matrix = translate(matrix, t);

	// yaw, pitch, roll
	float yaw = r.x;
	float pitch = r.y;
	float roll = r.z;

	matrix = matrix * alignment_matrix;
	matrix = rotate(matrix, radians(yaw), vec3(0, 1, 0));
	matrix = rotate(matrix, radians(pitch), vec3(1, 0, 0));
	matrix = rotate(matrix, radians(roll), vec3(0, 0, 1));

	matrix = scale(matrix, vec3(s));

	return matrix;
}

glm::mat4 Maths::createRotationMatrix(const Camera& camera)
{
	mat4 matrix = mat4(1);

	// Rotate pitch, then yaw
	float yaw = camera.getRotation().x;
	float pitch = camera.getRotation().y;

	matrix = rotate(matrix, radians(-yaw), vec3(0, 1, 0));
	matrix = rotate(matrix, radians(-pitch), vec3(1, 0, 0));
	return matrix;
}


mat4 Maths::createViewMatrix(const Camera& camera)
{
	mat4 matrix = mat4(1);
	
	// Scale?

	// Rotate pitch, then yaw
	float pitch = camera.getRotation().y;
	float yaw = camera.getRotation().x;

	matrix = rotate(matrix, radians(pitch), vec3(1, 0, 0));
	matrix = rotate(matrix, radians(yaw), vec3(0, 1, 0));

	matrix = translate(matrix, -camera.getPosition());
	return matrix;
}

/* View matrix but without translation component */
mat4 Maths::createSkyViewMatrix(const Camera& camera)
{
	mat4 matrix = mat4(1);
	// Rotate pitch, then yaw
	float pitch = camera.getRotation().y;
	float yaw = camera.getRotation().x;

	matrix = rotate(matrix, radians(pitch), vec3(1, 0, 0));
	matrix = rotate(matrix, radians(yaw), vec3(0, 1, 0));

	return matrix;
}

vec3 Maths::interpolateColor(vec3 c1, vec3 c2, float fraction) 
{
	return (1.f - fraction) * c1 + (fraction * c2);
}

vec2 Maths::pixelToScreenCoords(vec2 pixel_coords)
{
	float x = (2.f * pixel_coords.x / (float)Engine::instance->screen_width) - 1.f;
	float y = (-2.f * pixel_coords.y / (float)Engine::instance->screen_height) + 1.f;
	return {x, y};
}
