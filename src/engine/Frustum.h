#pragma once
#include <glm/glm.hpp>
#include <vector>

class Camera;

class Frustum {
private:
	static const glm::vec4 forward;
	static const glm::vec4 up;

	float near_plane_;
	float far_plane_;

	float near_width_, far_width_, near_height_, far_height_;

public:
	glm::vec3 far_top_right, far_top_left, 
			  far_bottom_right, far_bottom_left,
		      near_top_right, near_top_left,
		      near_bottom_right, near_bottom_left;

	Frustum();
	Frustum(float false_near, float near_plane, float far_plane);

	void update(const Camera& camera);

	/**
	 * Calculates frustum corner position in world space,
	 */
	glm::vec3 calculateFrustumCorner(
		glm::vec3 start_point,
		glm::vec3 direction,
		float distance
	);

	std::vector<glm::vec4> getCorners();
};
