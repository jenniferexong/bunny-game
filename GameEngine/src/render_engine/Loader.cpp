
#include <GL/glew.h>

#include "Loader.h"

#include <stb_image/stb_image.h>

#include <iostream>

#include "../Location.h"

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
	cout << "Loaded: " << indices.size() << ", id: " << vao_id << endl;
	return { vao_id, (int) indices.size() };
}

/* Loads data from an obj file into a VAO */
Mesh Loader::loadToVao(const string& obj_file)
{
	WavefrontData data = WavefrontData(obj_file);
	int vao_id = createVao();
	storeInAttributeList(AttributeLocation::Position, 3, data.positions);
	storeInAttributeList(AttributeLocation::Normal, 3, data.normals);
	storeInAttributeList(AttributeLocation::Texture, 2, data.texture_coords);

	bindIbo(data.indices);
	unbindVao(); // unbinding
	return { vao_id, (int) data.indices.size() };
}

int Loader::loadTexture(const string& file_name)
{
	// Loading the image
	stbi_set_flip_vertically_on_load(0); // IF UPSIDE DOWN TEXTURE, CHANGE THIS
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(file_name.c_str(), &width, &height, &bpp, 4);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (buffer) { // if buffer contains data 
		stbi_image_free(buffer);
	}

	printf("Loaded texture: %s, %d\n", file_name.c_str(), texture_id);
	_textures.push_back(&texture_id);
	return texture_id;
}

/* Returns the id of a vao */
int Loader::createVao()
{
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	_vaos.push_back(&vao_id); // add to vector of vaos

	glBindVertexArray(vao_id);
	return vao_id;
}

void Loader::storeInAttributeList(int attrib_num, int coord_size, const vector<float>& data)
{
	GLuint vbo_id;
	glGenBuffers(1, &vbo_id);
	_vbos.push_back(&vbo_id); // add to vector of vaos

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_num, coord_size, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIbo(const vector<int>& indices)
{
	GLuint ibo_id;
	glGenBuffers(1, &ibo_id);
	_vbos.push_back(&ibo_id); // add to vector of vaos
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Loader::unbindVao()
{
	glBindVertexArray(0);
}

/* Clean up VAOs and VBOs and Textures */
Loader::~Loader()
{
	for (GLuint *vao : _vaos) {
		glDeleteVertexArrays(1, vao);
	}
	for (GLuint *vbo : _vbos) {
		glDeleteBuffers(1, vbo);
	}
	for (GLuint* texture : _textures) {
		glDeleteTextures(1, texture);
	}
}
