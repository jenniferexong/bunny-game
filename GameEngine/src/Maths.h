#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "environment/Camera.h"

namespace Maths {
	glm::vec3 getLookDirection(vec3 initial_direction, vec3 rotation, glm::mat4 alignment_matrix);
	glm::mat4 getAlignmentRotation(vec3 surface_normal);

	glm::mat4 createTransformationMatrix(glm::vec3 t, glm::vec3 r, float s, glm::mat4 alignment_matrix);
	glm::mat4 createTransformationMatrix(glm::vec2 t, float r, glm::vec2 s);
	glm::mat4 createViewMatrix(const Camera& camera);
	glm::mat4 createRotationMatrix(const Camera& camera);
	glm::mat4 createSkyViewMatrix(const Camera& camera);
};

