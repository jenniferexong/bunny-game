#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

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
	float getHeight(int x, int z, int image_size, const unsigned char* buffer);
	glm::vec3 getPosition(int x, int z, int image_size, const std::vector<float>& positions);
	glm::vec3 calculateNormal(int x, int z, int image_size, const std::vector<float>& positions);

public:
	Terrain() : x_(0), z_(0) {}
	Terrain(int grid_x, int grid_z, TerrainTexture texture, const std::string& height_map);

	float getX() const { return x_; }
	float getZ() const { return z_; }
	Mesh getMesh() const { return mesh_; }
	TerrainTexture getTexture() const { return texture_; }
};
