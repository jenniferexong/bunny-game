#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Maths {
	using namespace glm;
	
	/* Helper function: uses Euler angles for rotation */
	mat4 createTransformationMatrix(const vec3 t, const vec3 r, const float s) {
		mat4 matrix = mat4(1);

		matrix = translate(matrix, t);

		// yaw, pitch, roll
		float yaw = r.x;
		float pitch = r.y;
		float roll = r.z;

		matrix = rotate(matrix, radians(yaw), vec3(0, 1, 0));
		matrix = rotate(matrix, radians(pitch), vec3(0, 0, 1));
		matrix = rotate(matrix, radians(roll), vec3(1, 0, 0));

		matrix = scale(matrix, vec3(s));

		return matrix;
	}
};

