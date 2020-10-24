#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Mesh.h"
#include "WavefrontData.h"

using std::vector;


class Loader {
private:
	vector<GLuint *> m_vaos;
	vector<GLuint *> m_vbos;
	vector<GLuint *> m_textures;

	int createVao(); // returns an id
	void storeInAttributeList(int attrib_num, int coord_size, const vector<float>& data);
	void bindIbo(const vector<int>& indices);
	void unbindVao();

public:
	Mesh loadToVao(const vector<float>& positions, const vector<float>& texture_coords, const vector<int>& indices);
	Mesh loadToVao(WavefrontData& data);
	int loadTexture(const std::string& file_name);
	void destroy();
};
