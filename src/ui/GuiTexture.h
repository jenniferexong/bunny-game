#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <functional>

#include "GuiBound.h"

class GuiTexture {
private:
	GuiBound bound_;

	glm::vec4 color_ = glm::vec4(1);
	int texture_;
	bool has_texture_ = false;

	bool flip_vertically_ = false;
	bool has_transparency_ = true;
	
public:
	GuiTexture(): texture_(-1) {}

	GuiTexture(int texture, GuiBound bound): 
		bound_(bound),
		texture_(texture),
		has_texture_(true)
	{}

	GuiTexture(glm::vec4 color, GuiBound bound):
		bound_(bound),
		color_(color),
		texture_(-1)
	{}

	~GuiTexture() = default;

	int getTexture() const { return texture_; }
	const GuiBound& getBound() const { return bound_; }
	glm::vec2 getPosition() const { return bound_.getPosition(); }
	float getRotation() const { return bound_.getRotation(); }
	glm::vec2 getScale() const { return bound_.getScale(); }
	glm::vec4 getColor() const { return color_; }

	bool flipVertically() const { return flip_vertically_; }
	bool hasTransparency() const { return has_transparency_; }
	bool hasTexture() const { return has_texture_; }

	void setColor(glm::vec4 color);
	void setTexture(int texture);
	void setPosition(glm::vec2 position) { bound_.setPosition(position); }
	void setRotation(float rotation) { bound_.setRotation(rotation); }
	void setScale(glm::vec2 scale) { bound_.setScale(scale); }

	void setFlipVertically() { flip_vertically_ = true; }

	void setTransparency(bool transparency) 
	{
		has_transparency_ = transparency; 
	}

	bool operator==(const GuiTexture& other) const 
	{
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
		bool operator()(
				const weak_ptr<GuiTexture>& a, 
				const weak_ptr<GuiTexture>& b) const
		{
			return a.lock() == b.lock();
		}
	};
}
