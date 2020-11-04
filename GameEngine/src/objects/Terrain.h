#pragma once

#include "../models/Mesh.h"
#include "../models/Texture.h"

class Terrain {
private:
	static const float size;
	static const int vertex_count;
	
	float x_, z_;
	Mesh mesh_;
	TerrainTexture texture_;

	Mesh generate();

public:
	Terrain() : x_(0), z_(0) {}
	Terrain(int grid_x, int grid_z, TerrainTexture texture);

	float getX() const { return x_; }
	float getZ() const { return z_; }
	Mesh getMesh() const { return mesh_; }
	TerrainTexture getTexture() const { return texture_; }
};
