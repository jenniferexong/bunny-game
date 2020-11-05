
#include "Terrain.h"

#include <iostream>
#include <stb_image/stb_image.h>

#include "../Application.h"

using std::vector;
using namespace glm;

const float Terrain::size = 300.f;
const float Terrain::max_height = 30.f;
const float Terrain::max_pixel_color = 255 * 3;

Terrain::Terrain(int grid_x, int grid_z, TerrainTexture texture, const string& height_map)
	: x_(float(grid_x) * size), z_(float(grid_z) * size), texture_(std::move(texture))
{
	mesh_ = generate(height_map);
}

float Terrain::getHeight(int x, int z, int image_size, const unsigned char* buffer)
{
	if (x < 0 || x >= image_size || z < 0 || z >= image_size)
		return 0;

	int index = 3 * (x * image_size + z); // 3 channels

	// value in range [0, max_pixel_color]
	float r = buffer[index];
	float g = buffer[index + 1];
	float b = buffer[index + 2];
	float value = r + g + b;
	
	// value in range [-max_pixel_color/2, max_pixel_color/2]
	value -= max_pixel_color / 2;

	// value in range [-1, 1]
	value /= max_pixel_color / 2;

	// value in range [-max_height, max_height]
	float height = value * max_height;

	return height;
}

/**
 * Calculates the normal of the vertex based on the normal of the 4 neighbouring faces
 */
vec3 Terrain::calculateNormal(int x, int z, int image_size, const vector<float>& positions)
{
	vec3 x_offset = vec3(1, 0, 0);
	vec3 z_offset = vec3(0, 0, 1);
	vec3 position = getPosition(x, z, image_size, positions);
	vec3 north_position = z + 1 >= image_size ? position - z_offset : getPosition(x, z + 1, image_size, positions);
	vec3 east_position = x + 1 >= image_size ? position + x_offset : getPosition(x + 1, z, image_size, positions);
	vec3 south_position = z - 1 < 0 ? position + z_offset : getPosition(x, z - 1, image_size, positions);
	vec3 west_position = x - 1 < 0 ? position - x_offset : getPosition(x - 1, z, image_size, positions);

	vec3 north_direction = north_position - position;
	vec3 east_direction = east_position - position;
	vec3 south_direction = south_position - position;
	vec3 west_direction = west_position - position;

	vec3 ne_normal = normalize(cross(east_direction, north_direction));
	vec3 se_normal = normalize(cross(south_direction, east_direction));
	vec3 sw_normal = normalize(cross(west_direction, south_direction));
	vec3 nw_normal = normalize(cross(north_direction, west_direction));

	vec3 normal = ne_normal + se_normal + sw_normal + nw_normal;
	return normalize(normal);
}

vec3 Terrain::getPosition(int x, int z, int image_size, const std::vector<float>& positions)
{
	int index = 3 * (x * image_size + z); // 3 channels
	return vec3(positions.at(index), positions.at(index + 1), positions.at(index + 2));
}

/* Generates a mesh for the terrain */
Mesh Terrain::generate(const string& height_map)
{
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(height_map.c_str(), &width, &height, &bpp, 3); // 3 channels, rgb

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

			texture_coords.push_back((float)j / ((float)vertex_count - 1));
			texture_coords.push_back((float)i / ((float)vertex_count - 1));
		}
	}
	for (int i = 0; i < vertex_count; i++) {
		for (int j = 0; j < vertex_count; j++) {
			vec3 normal = calculateNormal(i, j, width, positions);
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
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

