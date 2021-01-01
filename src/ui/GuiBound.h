#pragma once

#include <glm/glm.hpp>

class GuiBound {
private:
	float x_min_, x_max_, y_min_, y_max_;

	glm::vec2 position_ = glm::vec2(0);
	float rotation_ = 0;
	// size of quad in relation to size of the screen
	glm::vec2 scale_; 

public:
	GuiBound(): x_min_(0), x_max_(0), y_min_(0), y_max_(0) {}

	/**
	 * @param point 
	 * - screen coordinates
	 */
	GuiBound(float left, float top, float width, float height);

	/**
	 * In opengl screen coordinates.
	 * position of (0, 0) is the center of the screen,
	 * scale of (1, 1) is the entire screen
	 */
	GuiBound(glm::vec2 position, glm::vec2 scale);
	GuiBound(const GuiBound& child, const GuiBound& parent);

	/**
	 * Pixel-screen coordinates (+x is right, +y is down)
	 */
	GuiBound(glm::ivec2 position, glm::ivec2 size);

	void setBounds();
	void setPosition(glm::vec2 position) { position_ = position; }
	void setScale(glm::vec2 scale) { scale_ = scale; }
	void setRotation(float rotation) { rotation_ = rotation; }

	bool containsPoint(glm::vec2 point) const;

	glm::vec2 getPosition() const { return position_; }
	glm::vec2 getTopLeft() const { return { x_min_, y_max_ }; }
	glm::vec2 getScale() const { return scale_; }
	float getRotation() const { return rotation_; }
};
