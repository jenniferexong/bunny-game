#pragma once

#include <glm/vec2.hpp>

#include <memory>
#include <string>
#include <vector>

class Light;

class Skybox {
private:
	int day_texture_id_ = -1;
	int night_texture_id_ = -1;

	// for blending two textures into each other
	float blend_factor_ = 0.f;
	int texture_1_ = -1;
	int texture_2_ = -1;

	std::vector<std::string> day_texture_names;
	std::vector<std::string> night_texture_names;

public:
	Skybox(const std::string& day_textures, const std::string& night_textures);
	Skybox() = default;
	~Skybox() = default;

	void update(std::weak_ptr<Light> sun);

	int getDayTexture() const { return day_texture_id_; }
	int getNightTexture() const { return night_texture_id_; }
	float getBlendFactor() const { return blend_factor_; }
	glm::ivec2 getTextures() const 
	{ 
		return glm::ivec2(texture_1_, texture_2_);
	}
};
