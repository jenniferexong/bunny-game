
#include "WavefrontData.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include "../Utility.h"

using namespace glm;
using namespace std;

WavefrontData::WavefrontData(const std::string& file_name) : model_center(glm::vec4(0)), model_radius(0)
{
	loadData(file_name);
}

void WavefrontData::loadData(const std::string& file_name)
{
	// temporary variables to store raw vertices and index data
	vector<vec3> in_positions, in_normals;
	vector<vec2> in_textures;

	std::ifstream obj_file(file_name);

	if (!obj_file.is_open())
		Error::file("obj", file_name);

	std::string line, type;
	std::string v1, v2, v3, v4; // values 

	while (std::getline(obj_file, line)) {
		std::istringstream str_stream(line);
		str_stream >> type; // get characters until first whitespace

		// reading positional data, normal data, and face data
		if (type == "v" || type == "vn" || type == "f") {
			str_stream >> v1 >> v2 >> v3;

			if (type == "v") // vertex positions
				in_positions.emplace_back(vec3(stof(v1), stof(v2), stof(v3)));

			else if (type == "vn") // vertex normals
				in_normals.emplace_back(vec3(stof(v1), stof(v2), stof(v3)));

			else if (type == "f") { // faces: reading indices
				processIndices(v1, in_positions, in_textures, in_normals);
				processIndices(v2, in_positions, in_textures, in_normals);
				processIndices(v3, in_positions, in_textures, in_normals);
				if (!str_stream.eof()) {
					str_stream >> v4;
					face = 4;
					processIndices(v4, in_positions, in_textures, in_normals);
				}
			}
		}

		else if (type == "vt") { // reading texture data
			str_stream >> v1 >> v2;
			in_textures.emplace_back(vec2(stof(v1), stof(v2)));
		}
	}
	obj_file.close();

	// reorder into triangles if needed
	if (face == 4) {
		vector<int> temp_indices;
		temp_indices = indices;
		indices.clear();

		for (int i = 0; i < (temp_indices.size() / 4); i++) {
			int v1 = temp_indices.at(i * 4 + 0);
			int v2 = temp_indices.at(i * 4 + 1);
			int v3 = temp_indices.at(i * 4 + 2);
			int v4 = temp_indices.at(i * 4 + 3);
			indices.push_back(v1);
			indices.push_back(v2);
			indices.push_back(v3);
			indices.push_back(v1);
			indices.push_back(v3);
			indices.push_back(v4);
		}
	}

	// set the bounding sphere variables
	float diameter = glm::max(glm::max(max_.x - min_.x, max_.y - min_.y), max_.z - min_.z);
	model_radius = diameter/2; // vector so we can apply model transformations
	float centre_x = min_.x + ((max_.x - min_.x) / 2);
	float centre_y = min_.y + ((max_.y - min_.y) / 2);
	float centre_z = min_.z + ((max_.z - min_.z) / 2);
	model_center = vec4(centre_x, centre_y, centre_z, 1);
}

void WavefrontData::processIndices(const string& vertex, const vector<vec3>& in_positions, const vector<vec2>& in_textures, const vector<vec3>& in_normals)
{
	indices.push_back(current_index_);
	std::istringstream str(vertex);
	std::string num;

	// adding each number to indices (note they start from 1, not 0)
	std::getline(str, num, '/');
	vec3 position = in_positions.at(stoi(num) - 1);
	setMinMax(position);

	positions.push_back(position.x);
	positions.push_back(position.y);
	positions.push_back(position.z);

	std::getline(str, num, '/');
	if (num.empty()) { // no texture coordinate
		texture_coords.push_back(0);
		texture_coords.push_back(0);
	}
	else {
		vec2 texture_coord = in_textures.at(stoi(num) - 1);
		texture_coords.push_back(texture_coord.x);
		texture_coords.push_back(texture_coord.y);
	}

	std::getline(str, num);
	vec3 normal = in_normals.at(stoi(num) - 1);
	normals.push_back(normal.x);
	normals.push_back(normal.y);
	normals.push_back(normal.z);

	current_index_++;
}

void WavefrontData::setMinMax(vec3 position)
{
	min_.x = position.x < min_.x ? position.x : min_.x;
	min_.y = position.y < min_.y ? position.y : min_.y;
	min_.z = position.z < min_.z ? position.z : min_.z;

	max_.x = position.x > max_.x ? position.x : max_.x;
	max_.y = position.y > max_.y ? position.y : max_.y;
	max_.z = position.z > max_.z ? position.z : max_.z;
}
