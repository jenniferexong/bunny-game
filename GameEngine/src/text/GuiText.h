#pragma once

#include <string>

#include <glm/glm.hpp>

#include "FontType.h"

class GuiText {
private:
	static int next_id;

	int id_ = 0;
	
	std::string text_;
	float font_size_ = 1;
	glm::vec3 color_ = glm::vec3(0.f); // default black

	int mesh_vao_;
	int vertex_count_;

	glm::vec2 position_;
	float max_line_width_;
	int line_count_;

	FontType font_;

	bool centered_ = false;

public:

	/**
	 * Creates text, loads its mesh into a VAO, adds text to the screen.
	 * Positions and widths in screen size, where the range is [0, 1]
	 *		- top left (0, 0)
	 *		- bottom right (1, 0)
	 */
	GuiText(const std::string& text, float font_size, const FontType& font, 
			glm::vec2 position, float max_line_width, bool centered);

	// equality
	bool operator==(const GuiText& other) const {
		return id_ == other.id_;
	}
};
