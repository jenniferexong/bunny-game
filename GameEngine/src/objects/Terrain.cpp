
#include "Terrain.h"

#include <vector>

#include "../Application.h"

using std::vector;

const float Terrain::size = 300.f;
const int Terrain::vertex_count = 128;

Terrain::Terrain(int grid_x, int grid_z, TerrainTexture texture)
	: x_(float(grid_x) * size), z_(float(grid_z) * size), texture_(std::move(texture))
{
	mesh_ = generate();
}

/* Generates a mesh for the terrain */
Mesh Terrain::generate()
{
	// Imagine a square grid: setting the positions
	int count = vertex_count * vertex_count;
	vector<float> positions;
	vector<float> normals;
	vector<float> texture_coords;
	vector<int> indices;

	for (int i = 0; i < vertex_count; i++) {
		for (int j = 0; j < vertex_count; j++) {
			positions.push_back((float)j / ((float)vertex_count - 1) * size);
			positions.push_back(0);
			positions.push_back((float)i / ((float)vertex_count - 1) * size);

			// Normal of (0, 1, 0)
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);

			texture_coords.push_back((float)j / ((float)vertex_count - 1));
			texture_coords.push_back((float)i / ((float)vertex_count - 1));
		}
	}
	for (int gz = 0; gz < vertex_count - 1; gz++) {
		for (int gx = 0; gx < vertex_count - 1; gx++) {
			int top_left = (gz * vertex_count) + gx;
			int top_right = top_left + 1;
			int bottom_left = ((gz + 1) * vertex_count) + gx;
			int bottom_right = bottom_left + 1;
			indices.push_back(top_left);
			indices.push_back(bottom_left);
			indices.push_back(top_right);
			indices.push_back(top_right);
			indices.push_back(bottom_left);
			indices.push_back(bottom_right);
		}
	}
	return Application::loader.loadToVao(positions, normals, texture_coords, indices);
}

