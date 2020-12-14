#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

using glm::vec3;
using glm::vec2;
using glm::vec4;
using std::vector;

/* Stores positions, normals, texture coordinates and indices from an obj file */
struct WavefrontData {
	vector<float> positions;
	vector<float> normals;
	vector<float> texture_coords;
	vector<int> indices;

	// bounding sphere
	vec4 model_center;
	float model_radius;

	// 3 = triangles, 4 = quads
	int face = 3; 

	/* Loads in vertex data from an obj file */
	WavefrontData(const std::string& file_name);

private:
	int current_index_ = 0;

	// for calculating bounding sphere
	vec3 min_ = vec3(std::numeric_limits<float>::max());
	vec3 max_ = vec3(std::numeric_limits<float>::min());

	void loadData(const std::string& file_name);
	void setMinMax(vec3 position);
	void processIndices(
		const std::string& vertex,
		const vector<vec3>& in_positions,
		const vector<vec2>& in_textures,
		const vector<vec3>& in_normals
	);
};
