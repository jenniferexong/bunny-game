#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <functional>

class GuiTexture {
private:
	int texture_;
	glm::vec2 position_;
	float rotation_ = 0;
	glm::vec2 scale_; // size of quad in relation to size of the screen

	bool flip_vertically_ = false;
	bool has_transparency_ = true;
	
public:
	GuiTexture(): texture_(-1), position_(glm::vec2(0)), scale_(1) {}

	/**
	 * In opengl screen coordinates.
	 * position of (0, 0) is the center of the screen,
	 * scale of (1, 1) is the entire screen
	 */
	GuiTexture(int texture, glm::vec2 position, glm::vec2 scale)
		: texture_(texture), position_(position), scale_(scale) {}

	/**
	 * Pixel coordinates (+x is right, +y is up)
	 */
	GuiTexture(int texture, glm::ivec2 position, glm::ivec2 size);

	~GuiTexture() = default;

	int getTexture() const { return texture_; }
	glm::vec2 getPosition() const { return position_; }
	float getRotation() const { return rotation_; }
	glm::vec2 getScale() const { return scale_; }
	bool flipVertically() const { return flip_vertically_; }
	bool hasTransparency() const { return has_transparency_; }

	void setTexture(int texture) { texture_ = texture; }
	void setPosition(glm::vec2 position) { position_ = position; }
	void setRotation(float rotation) { rotation_ = rotation; }
	void setScale(glm::vec2 scale) { scale_ = scale; }

	void setFlipVertically() { flip_vertically_ = true; }
	void setTransparency(bool transparency) { has_transparency_ = transparency; }

	bool operator==(const GuiTexture& other) const {
		return texture_ == other.texture_;
	}
};

using std::weak_ptr;
namespace std {
	template <>
	struct hash<weak_ptr<GuiTexture>>
	{
		size_t operator()(const weak_ptr<GuiTexture>& gui_texture) const
		{
			return hash<int>()(gui_texture.lock()->getTexture());
		}
	};
	template <>
	struct equal_to<weak_ptr<GuiTexture>>
	{
		bool operator()(const weak_ptr<GuiTexture>& a, const weak_ptr<GuiTexture>& b) const
		{
			return a.lock() == b.lock();
		}
	};
}