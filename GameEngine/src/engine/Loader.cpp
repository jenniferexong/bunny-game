
#include <GL/glew.h>

#include "Loader.h"

#include <iostream>

#include "Utility.h"
#include "models/Mesh.h"
#include "models/WavefrontData.h"
#include "Location.h"
#include "FilePath.h"

using namespace std;
using namespace glm;

Mesh Loader::loadToVao(const vector<float>& positions, const vector<float> & normals,
	const vector<float>& texture_coords, const vector<int>& indices)
{
	int vao_id = createVao();
	storeInAttributeList(AttributeLocation::Position, 3, positions);
	storeInAttributeList(AttributeLocation::Normal, 3, normals);
	storeInAttributeList(AttributeLocation::Texture, 2, texture_coords);

	bindIbo(indices);
	unbindVao(); // unbinding
	return { vao_id, (int) indices.size(), 3 };
}

Mesh Loader::loadToVao(const vector<float>& positions, const vector<float>& normals, 
		const vector<float>& texture_coords, const vector<float>& tangents, const vector<int>& indices)
{
	int vao_id = createVao();
	storeInAttributeList(TerrainAttributeLocation::Position, 3, positions);
	storeInAttributeList(TerrainAttributeLocation::Normal, 3, normals);
	storeInAttributeList(TerrainAttributeLocation::Texture, 2, texture_coords);
	storeInAttributeList(TerrainAttributeLocation::Tangent, 3, tangents);

	bindIbo(indices);
	unbindVao(); // unbinding
	return { vao_id, (int) indices.size(), 3 };
}

/* Loading text mesh data to a vao (2d positions) */
int Loader::loadToVao(const vector<float>& positions, const vector<float>& texture_coords)
{
	int vao_id = createVao();
	storeInAttributeList(TextAttributeLocation::Position, 2, positions);
	storeInAttributeList(TextAttributeLocation::Texture, 2, texture_coords);
	unbindVao(); // unbinding
	return vao_id;
}


/* Loads data from an obj file into a VAO */
Mesh Loader::loadToVao(const string& obj_name)
{
	const string file_path = FilePath::model_path + obj_name + ".obj";
	WavefrontData data = WavefrontData(file_path);
	int vao_id = createVao();
	storeInAttributeList(AttributeLocation::Position, 3, data.positions);
	storeInAttributeList(AttributeLocation::Normal, 3, data.normals);
	storeInAttributeList(AttributeLocation::Texture, 2, data.texture_coords);

	bindIbo(data.indices);
	unbindVao(); 
	return { vao_id, (int) data.indices.size(), data.face};
}

/* Also sets the bounding sphere information for the mesh */
InstancedMesh Loader::loadToVaoInstanced(const string& obj_name)
{
	const string file_path = FilePath::model_path + obj_name + ".obj";
	WavefrontData data = WavefrontData(file_path);
	int vao_id = createVao();
	storeInAttributeList(AttributeLocation::Position, 3, data.positions);
	storeInAttributeList(AttributeLocation::Normal, 3, data.normals);
	storeInAttributeList(AttributeLocation::Texture, 2, data.texture_coords);

	bindIbo(data.indices);

	InstancedMesh mesh = InstancedMesh(vao_id, (int)data.indices.size(), data.face);
	mesh.setBoundingSphere(BoundingSphere(data.model_center, data.model_radius));

	int model_matrix_vbo = createInstancedVbo(AttributeLocation::ModelMatrixColumn1, 4, 16, 4);
	int brightness_vbo = createInstancedVbo(AttributeLocation::ModelBrightness, 1, 1, 0);
	mesh.setVbos(model_matrix_vbo, brightness_vbo);

	unbindVao(); 

	return mesh;
}

Mesh Loader::loadToVao(const vector<float>& positions, int dimensions)
{
	int vao_id = createVao();
	storeInAttributeList(AttributeLocation::Position, dimensions, positions);
	unbindVao();
	return { vao_id, (int) positions.size() / dimensions, 3 };
}

int Loader::loadCubeMap(std::vector<std::string> texture_names)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	// order of textures for faces must be: right, left, top, bottom, back, front
	for (int i = 0; i < 6; i++) {
		SkyboxTextureData data = SkyboxTextureData(texture_names.at(i));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.buffer);
		data.unload();
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	textures_.push_back(texture_id);
	Print::texture("cube map (" + texture_names.at(0) + ")", texture_id);
	return texture_id;
}


int Loader::loadTexture(const string& texture_name)
{
	const string file_path = FilePath::texture_path + texture_name + ".png";
	// Loading the image
	stbi_set_flip_vertically_on_load(1); // IF UPSIDE DOWN TEXTURE, CHANGE THIS
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(file_path.c_str(), &width, &height, &bpp, 4);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (buffer)
		stbi_image_free(buffer);

	Print::texture(texture_name, texture_id);
	textures_.push_back(texture_id);
	return texture_id;
}

int Loader::loadFontTexture(const string& texture_name)
{
	const string file_path = FilePath::font_path + texture_name + ".png";
	// Loading the image
	stbi_set_flip_vertically_on_load(1); // IF UPSIDE DOWN TEXTURE, CHANGE THIS
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(file_path.c_str(), &width, &height, &bpp, 4);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.f);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (buffer)
		stbi_image_free(buffer);

	Print::texture(texture_name, texture_id);
	textures_.push_back(texture_id);
	return texture_id;
}

/* Returns the id of a vao */
int Loader::createVao()
{
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	vaos_.push_back(vao_id); // add to vector of vaos

	glBindVertexArray(vao_id);
	return vao_id;
}

void Loader::storeInAttributeList(int attrib_num, int coord_size, const vector<float>& data)
{
	GLuint vbo_id;
	glGenBuffers(1, &vbo_id);
	vbos_.push_back(vbo_id); // add to vector of vaos

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_num, coord_size, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @param num_elements number of float in one block of data (eg. 16 in a 4x4 matrix)
 * @param blocks 4 blocks (of vec4) in a 4x4 matrix
 * @param offset offset in floats (eg. offset of 4 floats in a 4x4 matrix)
 */
int Loader::createInstancedVbo(int start_location, int blocks, int num_elements, int offset)
{
	GLuint vbo_id;
	glGenBuffers(1, &vbo_id);
	vbos_.push_back(vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	for (int i = 0; i < blocks; i++) {
		glVertexAttribPointer(start_location + i, blocks, GL_FLOAT, GL_FALSE, sizeof(float) * num_elements, (void*) (sizeof(float) * (i * offset)));
		glVertexAttribDivisor(start_location + i, 1); 
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo_id;
}


void Loader::bindIbo(const vector<int>& indices)
{
	GLuint ibo_id;
	glGenBuffers(1, &ibo_id);
	vbos_.push_back(ibo_id); // add to vector of vaos
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Loader::unbindVao()
{
	glBindVertexArray(0);
}

void Loader::deleteVao(GLuint vao)
{
	auto it = std::find(vaos_.begin(), vaos_.end(), vao);
	vaos_.erase(it);
	glDeleteVertexArrays(1, &vao);
}

/* Clean up VAOs and VBOs and Textures */
Loader::~Loader()
{
	glDeleteVertexArrays(vaos_.size(), vaos_.data());
	glDeleteBuffers(vbos_.size(), vbos_.data());
	glDeleteTextures(textures_.size(), textures_.data());
}


