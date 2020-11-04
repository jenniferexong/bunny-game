
#include "WavefrontData.h"

#include <sstream>
#include <iostream>
#include <fstream>

using std::string;

WavefrontData::WavefrontData(const std::string& file_name)
{
	loadData(file_name);
}

void WavefrontData::loadData(const std::string& file_name)
{
	// temporary variables to store raw vertices and index data
	//vector<int> in_pos_indices, in_texture_indices, in_norm_indices;
	vector<vec3> in_positions, in_normals;
	vector<vec2> in_textures;

	std::ifstream obj_file(file_name);

	if (obj_file.is_open()) {
		std::string line, type;
		std::string v1, v2, v3; // values 

		while (std::getline(obj_file, line)) {
			std::istringstream str_stream(line);
			str_stream >> type; // get characters until first whitespace

			// reading positional data, normal data, and face data
			if (type == "v" || type == "vn" || type == "f") {
				str_stream >> v1 >> v2 >> v3;

				if (type == "v") // vertex positions
					in_positions.push_back(vec3(stof(v1), stof(v2), stof(v3)));

				else if (type == "vn") // vertex normals
					in_normals.push_back(vec3(stof(v1), stof(v2), stof(v3)));

				else if (type == "f") { // faces: reading indices
					processIndices(v1, in_positions, in_textures, in_normals);
					processIndices(v2, in_positions, in_textures, in_normals);
					processIndices(v3, in_positions, in_textures, in_normals);
				}
			}

			else if (type == "vt") { // reading texture data
				str_stream >> v1 >> v2;
				in_textures.push_back(vec2(stof(v1), stof(v2)));
			}
		}
		obj_file.close();
	}
	else {
		std::cerr << "Error reading obj file: " << file_name << std::endl;
	}
}

void WavefrontData::processIndices(const string& vertex, const vector<vec3>& in_positions, const vector<vec2>& in_textures, const vector<vec3>& in_normals)
{
	indices.push_back(current_index_);
	std::istringstream str(vertex);
	std::string num;

	// adding each number to m_indices (note they start from 1, not 0)
	std::getline(str, num, '/');
	vec3 position = in_positions.at(stoi(num) - 1);
	positions.push_back(position.x);
	positions.push_back(position.y);
	positions.push_back(position.z);

	std::getline(str, num, '/');
	vec2 texture_coord = in_textures.at(stoi(num) - 1);
	texture_coords.push_back(texture_coord.x);
	texture_coords.push_back(texture_coord.y);

	std::getline(str, num);
	vec3 normal = in_normals.at(stoi(num) - 1);
	normals.push_back(normal.x);
	normals.push_back(normal.y);
	normals.push_back(normal.z);

	current_index_++;
}