#pragma once

#include <glm/glm.hpp>

class Camera;

class FrustumBox {
private:
	friend class Camera;

	static glm::vec4 forward;
	static glm::vec4 up;
	static float far_plane;
	static float near_plane;

	float far_width_, near_width_;
	float far_height_, near_height_;
	glm::vec4 right_plane_, left_plane_, top_plane_, bottom_plane_, far_plane_, near_plane_;

	glm::vec4 getPlaneEquation(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
	void update(const Camera& camera);
	bool containsPoint(glm::vec3);

public:
	FrustumBox();
};
