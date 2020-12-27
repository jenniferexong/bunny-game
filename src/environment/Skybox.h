#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <vector>

class Light;

class Skybox {
private:
	int day_texture_id_ = -1;
	int night_texture_id_ = -1;
	int dawn_texture_id_ = -1;

	// for blending two textures into each other
	float blend_factor_ = 0.f;
	int texture_1_ = -1;
	int texture_2_ = -1;

	static const glm::vec3 night_col;
	static const glm::vec3 dawn_col;
	static const glm::vec3 day_col;

	static const int night_start;
	static const int night_end;
	static const int day_start;
	static const int day_end;
	static const int dawn_start;
	static const int dawn_middle;
	static const int dawn_end;
	static const int dusk_start;
	static const int dusk_end;

public:
	Skybox(
		const std::string& day_textures,
		const std::string& night_textures,
		const std::string& dawn_textures
	);

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
