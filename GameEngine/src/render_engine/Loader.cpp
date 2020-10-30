
#include <GL/glew.h>

#include "Loader.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include "Texture.h"
#include "../AttributeLocation.h"

using namespace std;
using namespace glm;

Mesh Loader::loadToVao(const vector<float>& positions, const vector<float> & normals,
	const vector<float>& texture_coords, const vector<int>& indices)
{
	int vao_id = createVao();
	storeInAttributeList(ePosition, 3, positions);
	storeInAttributeList(eNormal, 3, normals);
	storeInAttributeList(eTexture, 2, texture_coords);

	bindIbo(indices);
	unbindVao(); // unbinding
	return Mesh(vao_id, indices.size());
}

/* Loads data from an obj file into a VAO */
Mesh Loader::loadToVao(const string& obj_file)
{
	WavefrontData data = WavefrontData(obj_file);
	int vao_id = createVao();
	storeInAttributeList(ePosition, 3, data.positions);
	storeInAttributeList(eNormal, 3, data.normals);
	storeInAttributeList(eTexture, 2, data.texture_coords);

	bindIbo(data.indices);
	unbindVao(); // unbinding
	return Mesh(vao_id, data.indices.size());
}

int Loader::loadTexture(const string& file_name)
{
	Texture texture = Texture(file_name);
	GLuint texture_id = texture.getId();
	m_textures.push_back(&texture_id);
	printf("t id: %d\n", texture_id);
	return texture_id;
}

/* Returns the id of a vao */
int Loader::createVao()
{
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	m_vaos.push_back(&vao_id); // add to vector of vaos

	glBindVertexArray(vao_id);
	return vao_id;
}

void Loader::storeInAttributeList(int attrib_num, int coord_size, const vector<float>& data)
{
	GLuint vbo_id;
	glGenBuffers(1, &vbo_id);
	m_vbos.push_back(&vbo_id); // add to vector of vaos

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_num, coord_size, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIbo(const vector<int>& indices)
{
	GLuint ibo_id;
	glGenBuffers(1, &ibo_id);
	m_vbos.push_back(&ibo_id); // add to vector of vaos
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
	for (GLuint *vao : m_vaos) {
		glDeleteVertexArrays(1, vao);
	}
	for (GLuint *vbo : m_vbos) {
		glDeleteBuffers(1, vbo);
	}
	for (GLuint* texture : m_textures) {
		glDeleteTextures(1, texture);
	}
}
