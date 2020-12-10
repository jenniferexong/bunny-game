#include "FrustumBox.h"

#include "renderers/MasterRenderer.h"
#include "../scene/Camera.h"
#include "Utility.h"
#include "Application.h"

using namespace glm;

float FrustumBox::far_plane = 450.f;
float FrustumBox::near_plane = -5.f;
vec4 FrustumBox::forward = vec4(0, 0, -1.f, 0);
vec4 FrustumBox::up = vec4(0, 1.f, 0, 0);

FrustumBox::FrustumBox() : far_width_(0), near_width_(0), far_height_(0), near_height_(0),
	right_plane_(vec4(0)), left_plane_(vec4(0)), top_plane_(vec4(0)), bottom_plane_(vec4(0)),
	far_plane_(vec4(0)), near_plane_(vec4(0))
{
	far_width_ = 1 * tan(radians(MasterRenderer::fov)) * far_plane;
	near_width_ = 1 * tan(radians(MasterRenderer::fov)) * MasterRenderer::near_plane;
}

void FrustumBox::update(const Camera& camera)
{
	float aspect_ratio = (float)engine->window_width / (float)engine->window_height;
	far_height_=  far_width_ / aspect_ratio;
	near_height_ = near_width_ / aspect_ratio;

	mat4 camera_rotation = Maths::createRotationMatrix(camera); // only rotation of camera
	vec3 forward_vector = normalize(vec3(camera_rotation * forward));
	vec3 up_vector = normalize(vec3(camera_rotation * up));
	vec3 down_vector = -up_vector;
	vec3 left_vector = normalize(cross(up_vector, forward_vector));
	vec3 right_vector = -left_vector;

	vec3 to_far = forward_vector * far_plane;
	vec3 to_near = forward_vector * near_plane;
	vec3 center_far = camera.getPosition() + to_far;
	vec3 center_near = camera.getPosition() + to_near;

	vec3 far_top_right = center_far + ((far_height_ / 2) * up_vector) + ((far_width_ / 2) * right_vector);
	vec3 far_top_left = center_far + ((far_height_ / 2) * up_vector) + ((far_width_ / 2) * left_vector);
	vec3 far_bottom_right = center_far + ((far_height_ / 2) * down_vector) + ((far_width_ / 2) * right_vector);
	vec3 far_bottom_left = center_far + ((far_height_ / 2) * down_vector) + ((far_width_ / 2) * left_vector);

	vec3 near_top_right = center_near + ((near_height_ / 2) * up_vector) + ((near_width_ / 2) * right_vector);
	vec3 near_top_left = center_near + ((near_height_ / 2) * up_vector) + ((near_width_ / 2) * left_vector);
	vec3 near_bottom_right = center_near + ((near_height_ / 2) * down_vector) + ((near_width_ / 2) * right_vector);
	vec3 near_bottom_left = center_near + ((near_height_ / 2) * down_vector) + ((near_width_ / 2) * left_vector);

	top_plane_ = getPlaneEquation(far_top_right, near_top_right, far_top_left);
	bottom_plane_ = getPlaneEquation(near_bottom_left, near_bottom_right, far_bottom_left);
	right_plane_ = getPlaneEquation(near_bottom_right, near_top_right, far_bottom_right);
	left_plane_ = getPlaneEquation(far_bottom_left, far_top_left, near_bottom_left);
	near_plane_ = getPlaneEquation(near_bottom_left, near_top_left, near_bottom_right);
	far_plane_ = getPlaneEquation(far_bottom_right, far_top_right, far_bottom_left);
}

/**
 * Calculates the plane. Normal points INTO the view frustum
 * Returns vec4(a, b, c, d) where ax + by + cz + d = 0
 */
glm::vec4 FrustumBox::getPlaneEquation(vec3 p0, vec3 p1, vec3 p2)
{
	vec3 v1 = p1 - p0;
	vec3 v2 = p2 - p0;
	vec3 normal = normalize(cross(v1, v2));

	float d = -dot(normal, p0);
	return vec4(normal, d);
}

bool FrustumBox::containsPoint(vec3 point)
{
	vec4 world_coordinate = vec4(point, 1.f);
	float top = dot(world_coordinate, top_plane_);
	float bottom = dot(world_coordinate, bottom_plane_);
	float left = dot(world_coordinate, left_plane_);
	float right = dot(world_coordinate, right_plane_);
	float near = dot(world_coordinate, near_plane_);
	float far = dot(world_coordinate, far_plane_);

	return top > 0 && bottom > 0 && left > 0 && right > 0 && near > 0 && far > 0;
}

