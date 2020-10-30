#pragma once

class Mesh {
private:
	int m_vao_id;
	int m_vertex_count;

public:
	// Constructors
	Mesh() : m_vao_id(-1), m_vertex_count(0) {}
	Mesh(int vao_id, int vertex_count) : m_vao_id(vao_id), m_vertex_count(vertex_count) {}

	// Getters
	inline int getId() const { return m_vao_id; }
	inline int getVertexCount() const { return m_vertex_count; }
};