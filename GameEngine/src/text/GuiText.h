#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "FontType.h"

class TextLoader;

class GuiText {
private:
	static TextLoader loader_;
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

	std::shared_ptr<FontType> font_ = nullptr;

	bool centered_ = false;

public:

	/**
	 * Creates text, loads its mesh into a VAO, adds text to the screen.
	 * Positions and widths in screen size, where the range is [0, 1]
	 *		- top left (0, 0)
	 *		- bottom right (1, 0)
	 */
	GuiText(const std::string& text, float font_size, std::shared_ptr<FontType> font, 
			glm::vec2 position, float max_line_width, bool centered);

	void updateText(const std::string& text);

	std::shared_ptr<FontType> getFont() const { return font_; }
	float getFontSize() const { return font_size_; }
	float getMaxLineWidth() const { return max_line_width_; }
	std::string getText() const { return text_; }
	glm::vec3 getColor() const { return color_; }
	glm::vec2 getPosition() const { return position_; }
	int getVao() const { return mesh_vao_; }
	int getVertexCount() const { return vertex_count_; }
	bool isCentered() const { return centered_; }

	void setMeshDataInfo(int vao, int vertex_count);
	void setColor(glm::vec3 color) { color_ = color; }
	void setLineCount(int count) { line_count_ = count; }
	
	// equality
	bool operator==(const GuiText& other) const {
		return id_ == other.id_;
	}

	bool operator< (const GuiText& other) const {
		return id_ < other.id_;
	}
};
