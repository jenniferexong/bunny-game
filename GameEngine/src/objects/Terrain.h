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
	Terrain() {}
	Terrain(int grid_x, int grid_z, TerrainTexture texture);

	inline float getX() const { return m_x; }
	inline float getZ() const { return m_z; }
	inline Mesh getMesh() const { return m_mesh; }
	inline TerrainTexture getTexture() const { return m_texture; }
};
