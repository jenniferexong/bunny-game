#include "Text.h"

Line::Line(double space_size, double font_size, double max_width)
{
	space_width_ = space_size * font_size;
	max_width_ = max_width;
}

// returns false upon failure to add word
bool Line::addWord(const Word& word)
{
	double length = word.getWidth();
	length += words_.empty() ? 0 : space_width_;
	if (current_line_width_ + length <= max_width_) {
		words_.push_back(word);
		current_line_width_ += length;
		return true;
	}
	return false;
}

void Word::addCharacter(const Character& character)
{
	characters_.emplace_back(character);
	width_ += character.getAdvance() * font_size_;
}

Character::Character(
	int id,
	dvec2 texture_coords,
	dvec2 texture_size,
	dvec2 offset,
	dvec2 quad_size,
	double x_advance
):
	id_(id),
	texture_coords_(texture_coords),
	offset_(offset),
	quad_size_(quad_size),
	x_advance_(x_advance)
{
	max_texture_coords_.x = texture_coords_.x + texture_size.x;
	max_texture_coords_.y = texture_coords.y - texture_size.y;
}
