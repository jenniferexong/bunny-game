#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>

#include "GuiBound.h"
#include "../util/Log.h"

class GuiTexture {
private:
	static std::unordered_map<std::string, int> textures;

    std::weak_ptr<GuiBound> bound_;

	glm::vec4 color_ = glm::vec4(1);
	int texture_;
	bool has_texture_ = false;

	bool flip_vertically_ = false;
	bool has_transparency_ = true;
	
public:
    static void loadTextures();
    static int getTexture(const std::string& name)
    {
        if (textures.find(name) == textures.end())
            Error::exit("GuiTexture::textures doesn't contain: " + name);

        return textures[name];
    }

	GuiTexture(): texture_(-1) {}

	GuiTexture(int texture): texture_(texture), has_texture_(true) {}
	GuiTexture(glm::vec4 color): color_(color), texture_(-1) {}

	GuiTexture(int texture, std::weak_ptr<GuiBound> bound): 
		bound_(bound),
		texture_(texture),
		has_texture_(true)
	{}

    GuiTexture(glm::vec4 color, std::weak_ptr<GuiBound> bound):
		bound_(bound),
		color_(color),
		texture_(-1)
	{}

	~GuiTexture() = default;

	int getTexture() const { return texture_; }
	const GuiBound& getBound() const { return *bound_.lock(); }
	glm::vec2 getPosition() const { return getBound().getPosition(); }
	float getRotation() const { return getBound().getRotation(); }
	glm::vec2 getScale() const { return getBound().getScale(); }
	glm::vec4 getColor() const { return color_; }

	bool flipVertically() const { return flip_vertically_; }
	bool hasTransparency() const { return has_transparency_; }
	bool hasTexture() const { return has_texture_; }

	void setColor(glm::vec4 color);
	void setTexture(int texture);
    void setBounds(std::weak_ptr<GuiBound> bound) { bound_ = bound; }
	void setPosition(glm::vec2 position) { bound_.lock()->setPosition(position); }
	void setRotation(float rotation) { bound_.lock()->setRotation(rotation); }
	void setScale(glm::vec2 scale) { bound_.lock()->setScale(scale); }

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
