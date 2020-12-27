#pragma once

#include <glm/glm.hpp>

#include "Frustum.h"

class Camera;

class FrustumBox {
private:
	friend class Camera;

	static const float far_plane;
	static const float near_plane;

	Frustum frustum_;

	glm::vec4 right_plane_, left_plane_,
			  top_plane_, bottom_plane_,
			  far_plane_, near_plane_;

	glm::vec4 getPlaneEquation(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
	void update(const Camera& camera);
	bool containsPoint(glm::vec3);

public:
	FrustumBox();
	~FrustumBox() = default;
};
