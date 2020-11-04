#pragma once

class Mesh {
private:
	int vao_id_;
	int vertex_count_;

public:
	// Constructors
	Mesh() : vao_id_(-1), vertex_count_(0) {}
	Mesh(int vao_id, int vertex_count) : vao_id_(vao_id), vertex_count_(vertex_count) {}

	// Getters
	inline int getId() const { return vao_id_; }
	inline int getVertexCount() const { return vertex_count_; }
};