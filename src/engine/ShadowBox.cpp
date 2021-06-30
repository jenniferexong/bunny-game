#include "ShadowBox.h"

#include <iostream>

#include "renderers/MasterRenderer.h"
#include "util/Log.h"
#include "Camera.h"

const float ShadowBox::offset = 10.f;
const vec4 ShadowBox::up = vec4(0, 1.f, 0, 0);
const vec4 ShadowBox::forward = vec4(0, 0, -1.f, 0);
const float ShadowBox::shadow_distance = 200.f;

using namespace glm;
using namespace std;

ShadowBox::ShadowBox()
{
	// By lowering shadow_distance (shortening the view frustrum),
	// shadow quality improves, but distant objects won't cast shadows.
	frustum_ = Frustum(
		MasterRenderer::near_plane,
		MasterRenderer::near_plane,
		shadow_distance
	);
} 
void ShadowBox::update(const Camera& camera, glm::mat4 light_view)
{
	light_view_matrix_ = light_view;
	frustum_.update(camera);
	std::vector<vec4> points = frustum_.getCorners();
	for (unsigned int i = 0; i < points.size(); i++)
		points[i] = light_view_matrix_ * points[i];

	x_bounds.x = std::numeric_limits<float>::max();
	y_bounds.x = std::numeric_limits<float>::max();
	z_bounds.x = std::numeric_limits<float>::max();
	x_bounds.y = std::numeric_limits<float>::min();
	y_bounds.y = std::numeric_limits<float>::min();
	z_bounds.y = std::numeric_limits<float>::min();

	for (const auto& p: points) {
		if (p.x < x_bounds.x)
			x_bounds.x = p.x;
		else if (p.x > x_bounds.y)
			x_bounds.y = p.x;
		if (p.y < y_bounds.x)
			y_bounds.x = p.y;
		else if (p.y > y_bounds.y)
			y_bounds.y = p.y;
		if (p.z < z_bounds.x)
			z_bounds.x = p.z;
		else if (p.z > z_bounds.y)
			z_bounds.y = p.z;
	}
	z_bounds.y += offset;
}

vec3 ShadowBox::getCenter() const
{
	float x = (x_bounds.x + x_bounds.y) / 2.f;
	float y = (y_bounds.x + y_bounds.y) / 2.f;
	float z = (z_bounds.x + z_bounds.y) / 2.f;
	vec4 center = vec4(x, y, z, 1);
	mat4 inverse_light_view = inverse(light_view_matrix_);
	vec3 answer = vec3(inverse_light_view * center);
	return answer;
}
