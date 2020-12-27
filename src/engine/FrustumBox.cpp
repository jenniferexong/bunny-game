#include "FrustumBox.h"

#include "renderers/MasterRenderer.h"
#include "../scene/Camera.h"
#include "Utility.h"
#include "Application.h"

using namespace glm;

const float FrustumBox::far_plane = 450.f;
const float FrustumBox::near_plane = -10.f;

FrustumBox::FrustumBox(): 
	right_plane_(vec4(0)), left_plane_(vec4(0)),
	top_plane_(vec4(0)), bottom_plane_(vec4(0)),
	far_plane_(vec4(0)), near_plane_(vec4(0))
{
	frustum_ = Frustum(MasterRenderer::near_plane, near_plane, far_plane);
}

void FrustumBox::update(const Camera& camera)
{
	frustum_.update(camera);
	top_plane_ = getPlaneEquation(
		frustum_.far_top_right,
		frustum_.near_top_right,
		frustum_.far_top_left
	);
	bottom_plane_ = getPlaneEquation(
		frustum_.near_bottom_left,
		frustum_.near_bottom_right,
		frustum_.far_bottom_left
	);
	right_plane_ = getPlaneEquation(
		frustum_.near_bottom_right,
		frustum_.near_top_right,
		frustum_.far_bottom_right
	);
	left_plane_ = getPlaneEquation(
		frustum_.far_bottom_left,
		frustum_.far_top_left,
		frustum_.near_bottom_left
	);
	near_plane_ = getPlaneEquation(
		frustum_.near_bottom_left,
		frustum_.near_top_left,
		frustum_.near_bottom_right
	);
	far_plane_ = getPlaneEquation(
		frustum_.far_bottom_right, 
		frustum_.far_top_right,
		frustum_.far_bottom_left
	);
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

