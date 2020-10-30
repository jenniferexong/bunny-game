#pragma once

#include "../render_engine/Mesh.h"
#include "../render_engine/texture.h"

class Terrain {
private:
	static const float s_size;
	static const int s_vertex_count;
	
	float m_x, m_z;
	Mesh m_mesh;
	ModelTexture m_texture;

	Mesh generate();

public:
	Terrain(int grid_x, int grid_z, ModelTexture texture)
		: m_x(grid_x* s_size), m_z(grid_z* s_size), m_texture(texture) {}

	inline float getX() const { return m_x; }
	inline float getZ() const { return m_z; }
	inline Mesh getMesh() const { return m_mesh; }
	inline ModelTexture getTexture() const { return m_texture; }
};
