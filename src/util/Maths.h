#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

using glm::vec3;

namespace Maths {
	glm::vec3 getLookDirection(
			vec3 initial_direction, 
			vec3 rotation, 
			glm::mat4 alignment_matrix
	);
	glm::mat4 createTransformationMatrix(
			glm::vec3 t,
			glm::vec3 r,
			float s,
			glm::mat4 alignment_matrix
	);
	glm::mat4 getAlignmentRotation(vec3 surface_normal);
	glm::mat4 createTransformationMatrix(glm::vec2 t, float r, glm::vec2 s);
	glm::mat4 createViewMatrix(const Camera& camera);
	glm::mat4 createRotationMatrix(const Camera& camera);
	glm::mat4 createSkyViewMatrix(const Camera& camera);

	/**
	 * @param fraction amount of c2 in the final color
	 */
	glm::vec3 interpolateColor(vec3 c1, vec3 c2, float fraction);

	glm::vec2 pixelToScreenCoords(glm::vec2 pixel_coords);
};

