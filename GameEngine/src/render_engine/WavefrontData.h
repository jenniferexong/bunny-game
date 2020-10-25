#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <map>
#include <fstream>
//#include <sstream>


/* Stores positions, normals, texture coordinates and indices from an obj file */
struct WavefrontData {
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texture_coords;
	std::vector<int> indices;

	/* Loads in vertex data from an obj file */
	WavefrontData(const std::string& file_name) {
		std::vector<int> in_pos_indices, in_norm_indices;
	}
};
