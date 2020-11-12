
#include <GL/glew.h>

#include "Loader.h"

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
	return { vao_id, (int) indices.size(), 3 };
}

/* Loads data from an obj file into a VAO */
/*
Mesh Loader::loadToVao(const string& obj_file)
{
	WavefrontData data = WavefrontData(obj_file);
	int vao_id = createVao();
	storeInAttributeList(AttributeLocation::Position, 3, data.positions);
	storeInAttributeList(AttributeLocation::Normal, 3, data.normals);
	storeInAttributeList(AttributeLocation::Texture, 2, data.texture_coords);

	bindIbo(data.indices);
	unbindVao(); 
	return { vao_id, (int) data.indices.size(), data.face};
}
*/

/* Also sets the bounding sphere information for the mesh */
InstancedMesh Loader::loadToVao(const string& obj_file)
{
	WavefrontData data = WavefrontData(obj_file);
	int vao_id = createVao();
	storeInAttributeList(AttributeLocation::Position, 3, data.positions);
	storeInAttributeList(AttributeLocation::Normal, 3, data.normals);
	storeInAttributeList(AttributeLocation::Texture, 2, data.texture_coords);

	bindIbo(data.indices);

	int vbo_id = createModelMatrixVbo();
	int color_vbo_id = createModelColorVbo();
	unbindVao(); 
	InstancedMesh mesh = InstancedMesh(vao_id, vbo_id, color_vbo_id, (int)data.indices.size(), data.face);
	mesh.setBoundingSphere(BoundingSphere(data.model_center, data.model_radius));
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

	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	textures_.push_back(&texture_id);
	return texture_id;
}


int Loader::loadTexture(const string& file_name)
{
	// Loading the image
	stbi_set_flip_vertically_on_load(1); // IF UPSIDE DOWN TEXTURE, CHANGE THIS
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(file_name.c_str(), &width, &height, &bpp, 4);

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

	if (buffer) { 
		stbi_image_free(buffer);
	}

	printf("Loaded texture: %s, %d\n", file_name.c_str(), texture_id);
	textures_.push_back(&texture_id);
	return texture_id;
}


/* Returns the id of a vao */
int Loader::createVao()
{
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	vaos_.push_back(&vao_id); // add to vector of vaos

	glBindVertexArray(vao_id);
	return vao_id;
}

void Loader::storeInAttributeList(int attrib_num, int coord_size, const vector<float>& data)
{
	GLuint vbo_id;
	glGenBuffers(1, &vbo_id);
	vbos_.push_back(&vbo_id); // add to vector of vaos

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_num, coord_size, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int Loader::createModelMatrixVbo()
{
	GLuint vbo_id;
	glGenBuffers(1, &vbo_id);
	vbos_.push_back(&vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexAttribPointer(AttributeLocation::ModelMatrixColumn1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*) (sizeof(float) * 0));
	glVertexAttribDivisor(AttributeLocation::ModelMatrixColumn1, 1);

	glVertexAttribPointer(AttributeLocation::ModelMatrixColumn2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*) (sizeof(float) * 4));
	glVertexAttribDivisor(AttributeLocation::ModelMatrixColumn2, 1);

	glVertexAttribPointer(AttributeLocation::ModelMatrixColumn3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*) (sizeof(float) * 8));
	glVertexAttribDivisor(AttributeLocation::ModelMatrixColumn3, 1);

	glVertexAttribPointer(AttributeLocation::ModelMatrixColumn4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*) (sizeof(float) * 12));
	glVertexAttribDivisor(AttributeLocation::ModelMatrixColumn4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo_id;
}

int Loader::createModelColorVbo()
{
	GLuint vbo_id;
	glGenBuffers(1, &vbo_id);
	vbos_.push_back(&vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexAttribPointer(AttributeLocation::ModelColor, 1, GL_FLOAT, false, sizeof(float), nullptr);
	glVertexAttribDivisor(AttributeLocation::ModelColor, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo_id;
}


void Loader::bindIbo(const vector<int>& indices)
{
	GLuint ibo_id;
	glGenBuffers(1, &ibo_id);
	vbos_.push_back(&ibo_id); // add to vector of vaos
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
	for (GLuint *vao : vaos_) {
		glDeleteVertexArrays(1, vao);
	}
	for (GLuint *vbo : vbos_) {
		glDeleteBuffers(1, vbo);
	}
	for (GLuint* texture : textures_) {
		glDeleteTextures(1, texture);
	}
}
