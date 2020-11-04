
#include "WavefrontData.h"

#include <sstream>

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

void WavefrontData::loadData2(const std::string& file_name)
{
	// temporary variables to store raw vertices and index data
	vector<int> in_pos_indices, in_texture_indices, in_norm_indices;
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
					processIndices2(v1, &in_pos_indices, &in_texture_indices, &in_norm_indices);
					processIndices2(v2, &in_pos_indices, &in_texture_indices, &in_norm_indices);
					processIndices2(v3, &in_pos_indices, &in_texture_indices, &in_norm_indices);
				}
			}

			else if (type == "vt") { // reading texture data
				str_stream >> v1 >> v2;
				in_textures.push_back(vec2(stof(v1), stof(v2)));
			}
		}
		obj_file.close();

		// Processing the raw data: reorders the vertex positions, textures, normals by iterating through 
		// the indices data.
		//
		// Uses a map to keep track of unique vertices: A vertex is represented by the indices of its
		// position, texture and normal. eg. a tuple<0, 1, 2> represents a vertex with position of 
		// index 0 from in_positions, a texture coordinate of index 1 from in_textures, and a normal
		// of index 2 from in_normals.
		//
		// The value in the map is the index of the whole vertex in m_indices
		std::map<std::tuple<int, int, int>, int> unique_vertices;
		int current_index = 0;
		int num_vertices = in_pos_indices.size();

		// iterating through raw vertex data
		for (int i = 0; i < num_vertices; i++) {
			int pos_index = in_pos_indices.at(i);
			int texture_index = in_texture_indices.at(i);
			int norm_index = in_norm_indices.at(i);

			std::tuple<int, int, int> vertex = std::make_tuple(pos_index, texture_index, norm_index);

			// Check if the map already has this vertex, add corresponding index to indices
			if (unique_vertices.find(vertex) != unique_vertices.end()) {
				indices.push_back(unique_vertices.at(vertex));
			}
			else { // New, unique vertex
				unique_vertices.insert({ vertex, current_index });

				// Add position, texture, normal data to their respective vectors
				vec3 pos = in_positions.at(pos_index);
				vec2 texture = in_textures.at(texture_index);
				vec3 norm = in_normals.at(norm_index);

				positions.push_back(pos.x);
				positions.push_back(pos.y);
				positions.push_back(pos.z);
				texture_coords.push_back(texture.x);
				texture_coords.push_back(texture.y);
				normals.push_back(norm.x);
				normals.push_back(norm.y);
				normals.push_back(norm.z);

				indices.push_back(current_index);
				current_index++;
			}
		}
	}

	else {
		std::cerr << "Could not open obj file for reading" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void WavefrontData::processIndices2(std::string to_split, vector<int>* pos_indices,
	vector<int>* texture_indices, vector<int>* norm_indices) {
	std::istringstream str_stream(to_split);
	std::string num;

	// adding each number to m_indices (note they start from 1, not 0)
	std::getline(str_stream, num, '/');
	pos_indices->push_back(stoi(num) - 1);
	std::getline(str_stream, num, '/');
	texture_indices->push_back(stoi(num) - 1);
	std::getline(str_stream, num);
	norm_indices->push_back(stoi(num) - 1);
}