#include "Skybox.h"

#include <fstream>

#include "Light.h"
#include "../game-manager/FilePath.h"
#include "../util/Maths.h"
#include "../game-manager/Application.h"

using glm::vec3;
const vec3 Skybox::night_col = vec3(0.149, 0.266, 0.592);
const vec3 Skybox::dawn_col = vec3(0.756, 0.505, 0.545);
const vec3 Skybox::day_col = vec3(0.8f);

const int Skybox::night_start = 0;
const int Skybox::night_end = 8000;
const int Skybox::dawn_start = night_end;
const int Skybox::dawn_middle = night_end + (day_start - night_end) / 2;
const int Skybox::dawn_end = 12000;
const int Skybox::day_start = dawn_end;
const int Skybox::day_end = 20000;
const int Skybox::dusk_start = day_end;
const int Skybox::dusk_end = 24000;

/**
 * Reads a data file containing the names of the textures for the cubemap/s
 * order of textures for faces must be: right, left, top, bottom, back, front
 */
Skybox::Skybox(
	const std::string& day_textures,
	const std::string& night_textures,
	const std::string& dawn_textures)
{
	Log::init("Skybox", false);

	// load the textures
	day_texture_id_ = Engine::instance->loader->loadCubeMap(day_textures);
	night_texture_id_ = Engine::instance->loader->loadCubeMap(night_textures);
	dawn_texture_id_ = Engine::instance->loader->loadCubeMap(dawn_textures);

	Log::init("Skybox", true);
}

void Skybox::update(std::weak_ptr<Light> sun)
{
	float time = app->timer->time;

	vec3 light_col = vec3(0.8);
	// Night
	if (time >= night_start && time < night_end) {
		texture_1_ = night_texture_id_;
		texture_2_ = night_texture_id_;
		blend_factor_ = (time - night_start) / (night_end - 0);
		light_col = night_col;
	}
	// dawn starting
	else if (time >= dawn_start && time < dawn_middle) {
		texture_1_ = night_texture_id_;
		texture_2_ = dawn_texture_id_;
		blend_factor_ = (time - dawn_start) / (dawn_middle - dawn_start);
		light_col = Maths::interpolateColor(night_col, dawn_col, blend_factor_);
	}
	// dawn ending
	else if (time >= dawn_middle && time < dawn_end) {
		texture_1_ = dawn_texture_id_;
		texture_2_ = day_texture_id_;
		blend_factor_ = (time - dawn_middle) / (dawn_end - dawn_middle);
		light_col = Maths::interpolateColor(dawn_col, day_col, blend_factor_);
	}
	// day
	else if (time >= day_start && time < day_end) {
		texture_1_ = day_texture_id_;
		texture_2_ = day_texture_id_;
		blend_factor_ = (time - day_start) / (day_end - day_start);
	}
	// sunset
	else if (time >= dusk_start && time < dusk_end){
		texture_1_ = day_texture_id_;
		texture_2_ = night_texture_id_;
		blend_factor_ = (time - dusk_start) / (dusk_end - dusk_start);
		light_col = Maths::interpolateColor(day_col, night_col, blend_factor_);
	}
	sun.lock()->setColor(light_col);
}
