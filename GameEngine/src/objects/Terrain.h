#pragma once

#include <string>

#include "../models/Mesh.h"
#include "../models/Texture.h"

class Terrain {
private:
	static const float size;
	//static const int vertex_count;
	static const float max_height;
	static const float max_pixel_color;
	
	float x_, z_;
	Mesh mesh_;
	TerrainTexture texture_;

	Mesh generate(const std::string& height_map);
	float getHeight(int x, int y, int image_size, const unsigned char* buffer);

public:
	Terrain() : x_(0), z_(0) {}
	Terrain(int grid_x, int grid_z, TerrainTexture texture, const std::string& height_map);

	float getX() const { return x_; }
	float getZ() const { return z_; }
	Mesh getMesh() const { return mesh_; }
	TerrainTexture getTexture() const { return texture_; }
};
