#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "../models/Mesh.h"
#include "../models/Texture.h"

class Terrain {
private:
	static const float size;
	static const float max_height;
	static const float max_pixel_color;
	
	int vertex_count_; // number of vertices on a side of the square
	float x_, z_;
	Mesh mesh_;
	TerrainTexture texture_;

	std::vector<float> heights_;
	std::vector<float> positions_;
	std::vector<float> normals_;

	std::vector<float> texture_coords_;
	std::vector<float> tangents_;

	Mesh generate(const std::string& height_map);

	template <class Value>
	Value barycentric(glm::vec3 point, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, Value v1, Value v2, Value v3) const;

	float calculateHeight(int row, int col, const unsigned char* buffer);

	glm::vec3 calculateAverageValue(int row, int col,
		void(Terrain::* calculate_function)(glm::ivec2 v0, glm::ivec2 v1, glm::ivec2 v2, std::vector<glm::vec3>& values));
	void calculateNormal(glm::ivec2 v0, glm::ivec2 v1, glm::ivec2 v2, std::vector<glm::vec3>& normals);
	void calculateTangent(glm::ivec2 v0, glm::ivec2 v1, glm::ivec2 v2, std::vector<glm::vec3>& tangents);

	glm::vec3 getPosition(int row, int col) const;
	glm::vec3 getNormal(int row, int col) const;
	glm::vec2 getTextureCoordinate(int row, int col) const;
	float getHeight(int x, int z) const;

	bool invalidVertex(int row, int col);

public:
	Terrain() = default;
	Terrain(int grid_x, int grid_z, TerrainTexture texture, const std::string& height_map);

	float getX() const { return x_; }
	float getZ() const { return z_; }
	float getHeightOfTerrain(float world_x, float world_z) const;
	glm::vec3 getNormalOfTerrain(float world_x, float world_z) const;
	Mesh getMesh() const { return mesh_; }
	TerrainTexture getTexture() const { return texture_; }
};
