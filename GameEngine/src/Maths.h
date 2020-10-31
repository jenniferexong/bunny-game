#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objects/Camera.h"

namespace Maths {
	glm::mat4 createTransformationMatrix(const glm::vec3 t, const glm::vec3 r, const float s);
	glm::mat4 createViewMatrix(const Camera& camera);
};

