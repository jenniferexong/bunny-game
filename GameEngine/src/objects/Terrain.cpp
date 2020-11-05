
#include "Terrain.h"

#include <iostream>
#include <vector>
#include <stb_image/stb_image.h>

#include "../Application.h"

using std::vector;

const float Terrain::size = 300.f;
//const int Terrain::vertex_count = 128;
const float Terrain::max_height = 20.f;
const float Terrain::max_pixel_color = 255 * 3;

Terrain::Terrain(int grid_x, int grid_z, TerrainTexture texture, const string& height_map)
	: x_(float(grid_x) * size), z_(float(grid_z) * size), texture_(std::move(texture))
{
	mesh_ = generate(height_map);
}

float Terrain::getHeight(int x, int y, int image_size, const unsigned char* buffer)
{
	if (x < 0 || x > image_size || y < 0 || y > image_size)
		return 0;

	int index = 3 * (x * image_size + y); // 3 channels

	// value in range [0, max_pixel_color]
	float r = buffer[index];
	float g = buffer[index + 1];
	float b = buffer[index + 2];
	float rgb_value = r + g + b;
	
	// value in range [-max_pixel_color/2, max_pixel_color/2]
	rgb_value -= max_pixel_color / 2;

	// value in range [-1, 1]
	rgb_value /= max_pixel_color / 2;

	// value in range [-max_height, max_height]
	return rgb_value * max_height;
}

/* Generates a mesh for the terrain */
Mesh Terrain::generate(const string& height_map)
{
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(height_map.c_str(), &width, &height, &bpp, 3); // 3 channels, rgb
	int buffer_size = strlen((char*) buffer);
	//std::cout << "buffer: " << buffer_size << std::endl;
	//for (int i = 0; i < 30; i++)
		//std::cout << (int)buffer[i] << std::endl;

	int vertex_count = height;
	int count = vertex_count * vertex_count;
	vector<float> positions;
	vector<float> normals;
	vector<float> texture_coords;
	vector<int> indices;

	for (int i = 0; i < vertex_count; i++) {
		for (int j = 0; j < vertex_count; j++) {
			positions.push_back((float)j / ((float)vertex_count - 1) * size);
			positions.push_back(getHeight(i, j, width, buffer));
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
	if (buffer) { // if buffer contains data 
		stbi_image_free(buffer);
	}

	return Application::loader.loadToVao(positions, normals, texture_coords, indices);
}

