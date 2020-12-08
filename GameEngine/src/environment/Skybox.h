#pragma once

#include <string>
#include <vector>

class Skybox {
private:
	int day_texture_id_ = -1;
	int night_texture_id_ = -1;

	std::vector<std::string> day_texture_names;
	std::vector<std::string> night_texture_names;

public:
	Skybox() = default;
	~Skybox() = default;
	Skybox(const std::string& day_textures, const std::string& night_textures);

	int getDayTexture() const { return day_texture_id_; }
	int getNightTexture() const { return night_texture_id_; }
};
