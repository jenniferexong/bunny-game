#pragma once

#include <vector>
#include <string>

#include "../models/Mesh.h"
#include "WavefrontData.h"

using std::vector;

class Loader {
private:
	vector<GLuint *> vaos_;
	vector<GLuint *> vbos_;
	vector<GLuint *> textures_;

	int createVao(); // returns an id
	void storeInAttributeList(int attrib_num, int coord_size, const vector<float>& data);
	void bindIbo(const vector<int>& indices);
	void unbindVao();

public:
	~Loader();
	Mesh loadToVao(const vector<float>& positions, const vector<float>& normals, 
		const vector<float>& texture_coords, const vector<int>& indices);

	Mesh loadToVao(const std::string& obj_file);
	int loadTexture(const std::string& file_name);
};
