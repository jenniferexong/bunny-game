#pragma once

#include "../models/Mesh.h"
#include "../models/texture.h"

class Terrain {
private:
	static const float s_size;
	static const int s_vertex_count;
	
	float m_x, m_z;
	Mesh m_mesh;
	ModelTexture m_texture;

	Mesh generate();

public:
	Terrain(int grid_x, int grid_z, ModelTexture texture);

	inline float getX() const { return m_x; }
	inline float getZ() const { return m_z; }
	inline Mesh getMesh() const { return m_mesh; }
	inline ModelTexture getTexture() const { return m_texture; }
};
