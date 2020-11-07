
#include "Maths.h"

using namespace glm;


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
mat4 Maths::createTransformationMatrix(vec3 t, vec3 r, float s, mat4 alignment_matrix)
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
