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
 * Parameters in screen coordinates
 * @param position centre of the gui component
 */
GuiBound::GuiBound(vec2 position, vec2 scale): 
	position_(position),
	scale_(scale)
{
	setBounds();
}

GuiBound::GuiBound(ivec2 position, ivec2 size)
{
	float width = engine->screen_width;
	float height = engine->screen_height;
	position_ = Maths::pixelToScreenCoords(position);
	scale_ = vec2((float)size.x / width, (float)size.y / height);
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
