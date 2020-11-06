#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objects/Camera.h"

namespace Maths {
	glm::mat4 createTransformationMatrix(glm::vec3 t, glm::vec3 r, float s, glm::mat4 alignment_matrix);
	glm::mat4 createViewMatrix(const Camera& camera);
};

