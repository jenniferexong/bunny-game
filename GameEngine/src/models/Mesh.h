#pragma once

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
	int model_matrix_vbo_;

public:
	InstancedMesh(int vao_id, int model_matrix_vbo, int vertex_count, int face) : Mesh(vao_id, vertex_count, face),
		model_matrix_vbo_(model_matrix_vbo) {}

	int getModelMatrixVbo() const { return model_matrix_vbo_; }
};