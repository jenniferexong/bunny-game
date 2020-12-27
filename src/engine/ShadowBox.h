#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "Frustum.h"

class Camera;

using namespace glm;

class ShadowBox {
private:
	static const float offset;
	static const vec4 up;
	static const vec4 forward;
	static const float shadow_distance;

	Frustum frustum_;

	float far_height_, far_width_, near_height_, near_width_;

	vec2 x_bounds, y_bounds, z_bounds;
	mat4 light_view_matrix_;

public:
	ShadowBox();

	void update(const Camera& camera, mat4 light_view);
	vec2 getBoundsX() const { return x_bounds; }
	vec2 getBoundsY() const { return y_bounds; }
	vec2 getBoundsZ() const { return z_bounds; }
	vec3 getCenter() const;

	float getWidth() const { return x_bounds.y - x_bounds.x; }
	float getHeight() const { return y_bounds.y - y_bounds.x; }
	float getLength() const { return z_bounds.y - z_bounds.x; }
};

