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
	
	int vertex_count_; // number of vertices on a side of the square
	float x_, z_;
	std::vector<float> heights_;
	Mesh mesh_;
	TerrainTexture texture_;

	Mesh generate(const std::string& height_map);
	float getHeight(int row, int col, const unsigned char* buffer);
	glm::vec3 getPosition(int row, int col, const std::vector<float>& positions);
	glm::vec3 calculateNormal(int row, int col, const std::vector<float>& positions);
	float barycentric(glm::vec3 point, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float h1, float h2, float h3) const;

public:
	Terrain() = default;
	Terrain(int grid_x, int grid_z, TerrainTexture texture, const std::string& height_map);

	float getX() const { return x_; }
	float getZ() const { return z_; }
	float getHeightOfTerrain(float world_x, float world_z) const;
	float heights(int x, int z) const;
	Mesh getMesh() const { return mesh_; }
	TerrainTexture getTexture() const { return texture_; }
};
