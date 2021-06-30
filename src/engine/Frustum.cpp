#include "Frustum.h"

#include "renderers/MasterRenderer.h"
#include "Camera.h"
#include "Engine.h"
#include "util/Maths.h"
#include "util/Log.h"

using namespace glm;

const vec4 Frustum::forward = vec4(0, 0, -1.f, 0);
const vec4 Frustum::up = vec4(0, 1.f, 0, 0);

Frustum::Frustum(): 
	near_plane_(0), far_plane_(0),
	far_top_right(glm::vec3(0)), far_top_left(glm::vec3(0)),
	far_bottom_right(glm::vec3(0)), far_bottom_left(glm::vec3(0)),
	near_top_right(glm::vec3(0)), near_top_left(glm::vec3(0)),
	near_bottom_right(glm::vec3(0)), near_bottom_left(glm::vec3(0))
{}

/**
 * @param false_near 
 * - used to adjust size of near_width_
 * - larger false_near = larger near_width_
 */
Frustum::Frustum(float false_near, float near_plane, float far_plane)
{
	Frustum();
	near_plane_ = near_plane;
	far_plane_ = far_plane;
	far_width_ = 1 * tan(radians(MasterRenderer::fov)) * far_plane_;
	near_width_ = 1 * tan(radians(MasterRenderer::fov)) * false_near;
}

void Frustum::update(const Camera& camera)
{
	float aspect_ratio = (float)Engine::instance->window_width / (float)Engine::instance->window_height;
	far_height_=  far_width_ / aspect_ratio;
	near_height_ = near_width_ / aspect_ratio;

	mat4 rotation = Maths::createRotationMatrix(camera);
	vec3 forward_vector = normalize(vec3(rotation * forward));
	vec3 up_vector = normalize(vec3(rotation * up));
	vec3 down_vector = -up_vector;	
	vec3 left_vector = normalize(cross(up_vector, forward_vector));
	vec3 right_vector = -left_vector;	

	vec3 center_far = camera.getPosition() + (forward_vector * far_plane_);
	vec3 center_near = camera.getPosition() + (forward_vector * near_plane_);

	vec3 far_top = center_far + (far_height_/2 * up_vector);
	vec3 far_bottom = center_far + (far_height_/2 * down_vector);
	vec3 near_top = center_near + (near_height_/2 * up_vector);
	vec3 near_bottom = center_near + (near_height_/2 * down_vector);

	far_top_right = calculateFrustumCorner(far_top, right_vector, far_width_/2);
	far_top_left = calculateFrustumCorner(far_top, left_vector, far_width_/2);
	far_bottom_right = calculateFrustumCorner(far_bottom, right_vector, far_width_/2);
	far_bottom_left = calculateFrustumCorner(far_bottom, left_vector, far_width_/2);
	near_top_right = calculateFrustumCorner(near_top, right_vector, near_width_/2);
	near_top_left = calculateFrustumCorner(near_top, left_vector, near_width_/2);
	near_bottom_right = calculateFrustumCorner(near_bottom, right_vector, near_width_/2);
	near_bottom_left = calculateFrustumCorner(near_bottom, left_vector, near_width_/2);
}

vec3 Frustum::calculateFrustumCorner(
	vec3 start_point,
	vec3 direction,
	float distance) 
{
	vec3 point = start_point + (direction * distance);
	return point;
}

std::vector<vec4> Frustum::getCorners() 
{
	return {
		vec4(far_top_right, 1),
		vec4(far_top_left, 1),
		vec4(far_bottom_left, 1),
		vec4(far_bottom_right, 1),
		vec4(near_top_right, 1),
		vec4(near_top_left, 1),
		vec4(near_bottom_right, 1),
		vec4(near_bottom_left, 1)
	};
}
