#pragma once

#include "../models/Mesh.h"
#include "../models/Texture.h"

class Terrain {
private:
	static const float s_size;
	static const int s_vertex_count;
	
	float m_x, m_z;
	Mesh m_mesh;
	TerrainTexture m_texture;

	Mesh generate();

public:
	Terrain() : m_x(0), m_z(0) {}
	Terrain(int grid_x, int grid_z, TerrainTexture texture);

	float getX() const { return m_x; }
	float getZ() const { return m_z; }
	Mesh getMesh() const { return m_mesh; }
	TerrainTexture getTexture() const { return m_texture; }
};
