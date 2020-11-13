#pragma once

#include "BoundingSphere.h"

class Mesh {
protected:
	int vao_id_;
	int vertex_count_;
	int face_ = 3;

public:
	// Constructors
	Mesh() : vao_id_(-1), vertex_count_(0) {}
	Mesh(int vao_id, int vertex_count, int face) : vao_id_(vao_id), vertex_count_(vertex_count), face_(face) {}

	// Getters
	int getFace() const { return face_; }
	int getId() const { return vao_id_; }
	int getVertexCount() const { return vertex_count_; }
};

class InstancedMesh final: public Mesh {
private:
	BoundingSphere bounding_sphere_;
	int model_matrix_vbo_;
	int model_brightness_vbo_;

public:
	InstancedMesh(int vao_id, int vertex_count, int face) : Mesh(vao_id, vertex_count, face) {}	

	int getModelMatrixVbo() const { return model_matrix_vbo_; }
	int getModelBrightnessVbo() const { return model_brightness_vbo_; }
	BoundingSphere getBoundingSphere() const { return bounding_sphere_; }

	void setVbos(int model_matrix, int model_brightness);
	void setBoundingSphere(const BoundingSphere& sphere) { bounding_sphere_ = sphere; }
};