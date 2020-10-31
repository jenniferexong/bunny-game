
#include "Terrain.h"

#include <vector>

#include "../Application.h"

using std::vector;

const float Terrain::s_size = 800.f;
const int Terrain::s_vertex_count = 128;

Terrain::Terrain(int grid_x, int grid_z, ModelTexture texture)
	: m_x(grid_x* s_size), m_z(grid_z* s_size), m_texture(texture)
{
	m_mesh = generate();
}

/* Generates a mesh for the terrain */
Mesh Terrain::generate()
{
	// Imagine a square grid: setting the positions
	int count = s_vertex_count * s_vertex_count;
	vector<float> positions;
	vector<float> normals;
	vector<float> texture_coords;
	vector<int> indices;

	for (int i = 0; i < s_vertex_count; i++) {
		for (int j = 0; j < s_vertex_count; j++) {
			positions.push_back((float)j / ((float)s_vertex_count - 1) * s_size);
			positions.push_back(0);
			positions.push_back((float)i / ((float)s_vertex_count - 1) * s_size);

			// Normal of (0, 1, 0)
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);

			texture_coords.push_back((float)j / ((float)s_vertex_count - 1));
			texture_coords.push_back((float)i / ((float)s_vertex_count - 1));
		}
	}
	for (int gz = 0; gz < s_vertex_count - 1; gz++) {
		for (int gx = 0; gx < s_vertex_count - 1; gx++) {
			int top_left = (gz * s_vertex_count) + gx;
			int top_right = top_left + 1;
			int bottom_left = ((gz + 1) * s_vertex_count) + gx;
			int bottom_right = bottom_left + 1;
			indices.push_back(top_left);
			indices.push_back(bottom_left);
			indices.push_back(top_right);
			indices.push_back(top_right);
			indices.push_back(bottom_left);
			indices.push_back(bottom_right);
		}
	}
	return Application::s_loader.loadToVao(positions, normals, texture_coords, indices);
	/*
	const int count = s_vertex_count * s_vertex_count;
	float positions[count * 3];
	float normals[count * 3];
	float texture_coords[count * 2];
	int indices[6 * (s_vertex_count - 1) * (s_vertex_count - 1)];
	int vertex_pointer = 0;
	for (int i = 0; i < s_vertex_count; i++) {
		for (int j = 0; j < s_vertex_count; j++) {
			positions[vertex_pointer * 3] = (float)j / ((float)s_vertex_count - 1) * s_size;
			positions[vertex_pointer * 3 + 1] = 0;
			positions[vertex_pointer * 3 + 2] = (float)i / ((float)s_vertex_count - 1) * s_size;
			normals[vertex_pointer * 3] = 0;
			normals[vertex_pointer * 3 + 1] = 1;
			normals[vertex_pointer * 3 + 2] = 0;
			texture_coords[vertex_pointer * 2] = (float)j / ((float)s_vertex_count - 1);
			texture_coords[vertex_pointer * 2 + 1] = (float)i / ((float)s_vertex_count - 1);
			vertex_pointer++;
		}
	}
	int pointer = 0;
	for (int gz = 0; gz < s_vertex_count - 1; gz++) {
		for (int gx = 0; gx < s_vertex_count - 1; gx++) {
			int topLeft = (gz * s_vertex_count) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * s_vertex_count) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
	std::vector<float> pos, norm, text;
	std::vector<int> ind;
	for (int i = 0; i < sizeof(positions) / sizeof(float); i++) {
		pos.push_back(positions[i]);
		norm.push_back(normals[i]);
	}
	for (int i = 0; i < sizeof(texture_coords) / sizeof(float); i++) {
		text.push_back(texure_coords[i]);
	}
	for (int i = 0; i < sizeof(indices) / sizeof(int); i++) {
		ind.push_back(indices[i]);
	}
	return Application::s_loader.loadToVao(pos, norm, text, ind);
	*/
}

