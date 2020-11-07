#pragma once

#include <glm/glm.hpp>

class GuiTexture {
private:
	int texture_;
	glm::vec2 position_;
	float rotation_ = 0;
	glm::vec2 scale_; // size of quad in relation to size of the screen
	
public:
	GuiTexture(int texture, glm::vec2 position, glm::vec2 size);

	int getTexture() const { return texture_; }
	glm::vec2 getPosition() const { return position_; }
	float getRotation() const { return rotation_; }
	glm::vec2 getScale() const { return scale_; }

	void setPosition(glm::vec2 position) { position_ = position; }
	void setRotation(float rotation) { rotation_ = rotation; }
	void setScale(glm::vec2 scale) { scale_ = scale; }
};
