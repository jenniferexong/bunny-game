#include "Skybox.h"

#include <fstream>

#include "../Application.h"
#include "../Loader.h"

/**
 * Reads a data file containing the names of the textures for the cubemap/s
 * order of textures for faces must be: right, left, top, bottom, back, front
 */
Skybox::Skybox(const std::string& day_textures, const std::string& night_textures)
{
	std::string line;

	// day
	std::ifstream file("res/data/" + day_textures + ".txt");
	while (getline(file, line)) 
		day_texture_names.push_back(line);

	file.close();

	// night
	file = std::ifstream("res/data/" + night_textures + ".txt");
	while (getline(file, line)) 
		night_texture_names.push_back(line);

	file.close();

	// load the textures
	day_texture_id_ = app->loader->loadCubeMap(day_texture_names);
	night_texture_id_ = app->loader->loadCubeMap(night_texture_names);
}

