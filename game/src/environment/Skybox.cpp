#include "Skybox.h"

#include <fstream>

#include "Light.h"
#include "../game-manager/FilePath.h"


#include "../game-manager/Application.h"

/**
 * Reads a data file containing the names of the textures for the cubemap/s
 * order of textures for faces must be: right, left, top, bottom, back, front
 */
Skybox::Skybox(
	const std::string& day_textures,
	const std::string& night_textures)
{
	Log::init("Skybox", false);

	std::string line;

	// day
	std::ifstream file(FilePath::data_path + day_textures + ".txt");
	while (getline(file, line)) 
		day_texture_names.push_back(line);

	file.close();

	// night
	file = std::ifstream(FilePath::data_path + night_textures + ".txt");
	while (getline(file, line)) 
		night_texture_names.push_back(line);

	file.close();

	// load the textures
	day_texture_id_ = engine->loader->loadCubeMap(day_texture_names);
	night_texture_id_ = engine->loader->loadCubeMap(night_texture_names);

	Log::init("Skybox", true);
}

void Skybox::update(std::weak_ptr<Light> sun)
{
	float sunlight;

	float time = app->timer->time;
	// Night
	if (time >= 0 && time < 5000) {
		texture_1_ = night_texture_id_;
		texture_2_ = night_texture_id_;
		blend_factor_ = (time - 0) / (5000 - 0);
		sunlight = Light::min_sunlight;
	}
	// sunrise
	else if (time >= 5000 && time < 8000) {
		texture_1_ = night_texture_id_;
		texture_2_ = day_texture_id_;
		blend_factor_ = (time - 5000) / (8000 - 5000);
		sunlight = (
			Light::min_sunlight 
			+ (blend_factor_ * (Light::max_sunlight - Light::min_sunlight))
		);
	}
	// day
	else if (time >= 8000 && time < 21000) {
		texture_1_ = day_texture_id_;
		texture_2_ = day_texture_id_;
		blend_factor_ = (time - 8000) / (21000 - 8000);
		sunlight = Light::max_sunlight;
	}
	// sunset
	else {
		texture_1_ = day_texture_id_;
		texture_2_ = night_texture_id_;
		blend_factor_ = (time - 21000) / (24000 - 21000);
		sunlight = (
			Light::max_sunlight 
			- (blend_factor_ * (Light::max_sunlight - Light::min_sunlight))
		);
	}
	sun.lock()->setColor(glm::vec3(sunlight));
}


