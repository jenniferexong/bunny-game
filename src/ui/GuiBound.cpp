#include "GuiBound.h"
#include "../game-manager/Application.h"
#include "../util/Maths.h"

using namespace glm;

/**
 * Parameters in screen coordinates
 */
GuiBound::GuiBound(float left, float top, float width, float height)
{
	float w = engine->screen_width;
	float h = engine->screen_height;
	vec2 position = vec2(left + width / 2, top + height / 2);
	position_ = Maths::pixelToScreenCoords(position);
	scale_ = vec2((float)width / w, (float)height / h);

	setBounds();
}

/**
 * Parameters in screen coordinates, range [0, 1]
 * @param position centre of the gui component
 */
GuiBound::GuiBound(vec2 position, vec2 scale): 
	scale_(scale)
{
	position_.x = (2.f * position.x) - 1.f;
	position_.y = (-2.f * position.y) + 1.f;
	setBounds();
}

GuiBound::GuiBound(const GuiBound& child, const GuiBound& parent)
{
	vec2 pos = child.getPosition();
	vec2 scale = child.getScale();

	position_ = parent.getPosition() + (parent.getScale() * pos);
	scale_ = scale * parent.getScale();
	setBounds();
}

GuiBound::GuiBound(ivec2 position, ivec2 size, const GuiBound& parent)
{
    vec2 parent_pixel_size = 
        parent.scale_ * vec2(engine->screen_width, engine->screen_height);
    vec2 pos = vec2(position) / parent_pixel_size;
	position_.x = (2.f * pos.x) - 1.f;
	position_.y = (-2.f * pos.y) + 1.f;
	scale_ = vec2(
        (float)size.x / parent_pixel_size.x, (float)size.y / parent_pixel_size.y);
	setBounds();
}

void GuiBound::setBounds()
{
	x_min_ = position_.x - scale_.x;
	x_max_ = position_.x + scale_.x;
	y_min_ = position_.y - scale_.y;
	y_max_ = position_.y + scale_.y;
}

/**
 * point in pixel-screen coordinates
 */
bool GuiBound::containsPoint(vec2 point) const
{
	vec2 screen_coords = Maths::pixelToScreenCoords(point);
	return (
		screen_coords.x >= x_min_
		&& screen_coords.x <= x_max_
		&& screen_coords.y >= y_min_
		&& screen_coords.y <= y_max_
	);
}
