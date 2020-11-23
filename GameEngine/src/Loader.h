#pragma once

#include <stb_image/stb_image.h>

#include <vector>
#include <string>

#include "models/Mesh.h"

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

	Mesh loadToVao(const vector<float>& positions, const vector<float>& normals, 
		const vector<float>& texture_coords, const vector<float>& tangents, const vector<int>& indices);

	Mesh loadToVao(const vector<float>& positions, int dimensions);

	// Test mesh (2d positions)
	int loadToVao(const vector<float>& positions, const vector<float>& texture_coords);

	int loadCubeMap(std::vector<std::string> texture_files);

	int createInstancedVbo(int start_location, int blocks, int num_elements, int offset);

	Mesh loadToVao(const std::string& obj_name);
	InstancedMesh loadToVaoInstanced(const std::string& obj_name);
	int loadTexture(const std::string& texture_name);
};

struct SkyboxTextureData {
	int width, height;
	unsigned char* buffer;

	SkyboxTextureData(const std::string& file_name)
	{
		std::string file_path = "res/textures/skybox/" + file_name + ".png";
		stbi_set_flip_vertically_on_load(0); // IF UPSIDE DOWN TEXTURE, CHANGE THIS
		int bpp; // bits per pixel
		buffer = stbi_load(file_path.c_str(), &width, &height, &bpp, 4);
	}

	void unload()
	{
		if (buffer) 
			stbi_image_free(buffer);
	}
};
