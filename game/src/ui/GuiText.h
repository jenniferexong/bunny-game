#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

class FontType;
class TextLoader;

class GuiText {
private:
	static TextLoader loader_;
	static int next_id_;

	int id_ = 0;
	int mesh_vao_ = -1;
	int vertex_count_ = 0;
	
	std::string text_ = "";
	float font_size_ = 1;
	glm::vec3 color_ = glm::vec3(0.f); // default black
	glm::vec3 border_color_ = glm::vec3(1.0);

	float character_width_ = 0;
	float edge_transition_ = 0;
	float border_width_ = 0;
	float border_edge_transition_ = 0;

	glm::vec2 position_ = glm::vec2(0);
	float max_line_width_ = 0;
	int line_count_ = 0;

	std::shared_ptr<FontType> font_ = nullptr;

	bool centered_ = false;

public:

	/**
	 * Creates text, loads its mesh into a VAO, adds text to the screen.
	 * Positions and widths in screen size, where the range is [0, 1]
	 *		- top left (0, 0)
	 *		- bottom right (1, 0)
	 */
	GuiText(const std::string& text, float font_size, 
			std::shared_ptr<FontType> font, glm::vec2 position,
			float max_line_width, bool centered);
	~GuiText() = default;

	void updateText(const std::string& text);

	std::weak_ptr<FontType> getFont() const { return font_; }
	float getFontSize() const { return font_size_; }
	float getCharacterWidth() const { return character_width_; }
	float getEdgeTransition() const { return edge_transition_; }
	float getBorderWidth() const { return border_width_; }
	float getBorderEdgeTransition() const { return border_edge_transition_; }

	std::string getText() const { return text_; }
	glm::vec3 getColor() const { return color_; }
	glm::vec3 getBorderColor() const { return border_color_; }
	glm::vec2 getPosition() const { return position_; }
	bool isCentered() const { return centered_; }

	int getVao() const { return mesh_vao_; }
	int getVertexCount() const { return vertex_count_; }
	float getMaxLineWidth() const { return max_line_width_; }

	void setMeshDataInfo(int vao, int vertex_count);
	void setColor(glm::vec3 color) { color_ = color; }
	void setLineCount(int count) { line_count_ = count; }

	void setOutline(glm::vec3 color);
	void setGlow(glm::vec3 color);
	
	// equality
	bool operator==(const GuiText& other) const {
		return id_ == other.id_;
	}

	bool operator< (const GuiText& other) const {
		return id_ < other.id_;
	}
};
