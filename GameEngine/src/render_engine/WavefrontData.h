#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

using glm::vec3;
using glm::vec2;
using std::vector;

/* Stores positions, normals, texture coordinates and indices from an obj file */
struct WavefrontData {
	vector<float> positions;
	vector<float> normals;
	vector<float> texture_coords;
	vector<int> indices;
	int face = 3; // 3 = triangles, 4 = quads

	/* Loads in vertex data from an obj file */
	WavefrontData(const std::string& file_name);

	void loadData(const std::string& file_name);
	void processIndices(const std::string& vertex, const vector<vec3>& in_positions, const vector<vec2>& in_textures, const vector<vec3>& in_normals);

private:
	int current_index_ = 0;
};
