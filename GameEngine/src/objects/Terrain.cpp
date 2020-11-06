
#include "Terrain.h"

#include <iostream>
#include <cmath>
#include <stb_image/stb_image.h>

#include "../Application.h"

using std::vector;
using namespace glm;

const float Terrain::size = 510.f;
const float Terrain::max_height = 30.f;
const float Terrain::max_pixel_color = 255 * 3;

Terrain::Terrain(int grid_x, int grid_z, TerrainTexture texture, const string& height_map)
	: x_(float(grid_x) * size), z_(float(grid_z) * size), texture_(std::move(texture))
{
	mesh_ = generate(height_map);
}

float Terrain::getHeight(int row, int col, const unsigned char* buffer)
{
	if (row < 0 || row >= vertex_count_ || col < 0 || col >= vertex_count_)
		return 0;

	int index = 3 * (row * vertex_count_ + col); // 3 channels

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
vec3 Terrain::calculateNormal(int row, int col, const vector<float>& positions)
{
	vec3 x_offset = vec3(1, 0, 0);
	vec3 z_offset = vec3(0, 0, 1);
	vec3 position = getPosition(row, col, positions);
	vec3 north_position = row - 1 < 0 ? position - z_offset : getPosition(row - 1, col, positions);
	vec3 east_position = col + 1 >= vertex_count_ ? position + x_offset : getPosition(row, col + 1, positions);
	vec3 south_position = row + 1 >= vertex_count_ ? position + z_offset : getPosition(row + 1, col, positions);
	vec3 west_position = col - 1 < 0 ? position - x_offset : getPosition(row, col - 1, positions);

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

vec3 Terrain::getPosition(int row, int col, const std::vector<float>& positions)
{
	int index = 3 * (row * vertex_count_ + col); // 3 channels
	return vec3(positions.at(index), positions.at(index + 1), positions.at(index + 2));
}

float Terrain::heights(int x, int z) const
{
	int index = x * vertex_count_ + z;
	return heights_.at(index);
}

float test(vec3 p1, vec3 p2, vec3 p3, vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float answer = l1 * p1.y + l2 * p2.y + (1.f - l1 - l2) * p3.y;
	return answer;
}

float Terrain::getHeightOfTerrain(float world_x, float world_z) const
{
	float terrain_z = world_x - x_; // why is it reversed
	float terrain_x = world_z - z_;

	float grid_size = size / float(vertex_count_ - 1);
	int grid_x = (int) floor(terrain_x / grid_size);
	int grid_z = (int) floor(terrain_z / grid_size);

	// check if actually a valid grid on the terrain
	if (grid_x >= vertex_count_ - 1 || grid_z >= vertex_count_ - 1 || grid_x < 0 || grid_z < 0) 
		return 0;

	// get the x and z amount in its grid, in range [0, 1]
	float x = fmod(terrain_x, grid_size) / grid_size;
	float z = fmod(terrain_z, grid_size) / grid_size;

	// check which triangle the point in is, then use barycentric interpolation to find the height
	float answer = 0;
	if (x <= (1 - z)) { // left triangle
		answer = barycentric(vec3(x, 0, z), vec3(0, 0, 0), vec3(0, 0, 1), vec3(1, 0, 1),
			heights(grid_x, grid_z), heights(grid_x, grid_z + 1), heights(grid_x + 1, grid_z));
	}
	else { // right triangle
		answer = barycentric(vec3(x, 0, z), vec3(1, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1),
			heights(grid_x + 1, grid_z + 1), heights(grid_x + 1, grid_z), heights(grid_x, grid_z + 1));
	}
	return answer;
}

float Terrain::barycentric(vec3 point, vec3 p1, vec3 p2, vec3 p3, float h1, float h2, float h3) const
{
	const float area = 0.5f;
	float u = glm::length(glm::cross(p1 - point, p2 - point)) / area;
	float v = glm::length(glm::cross(p1 - point, p3 - point)) / area;
	float w = 1.f - u - v;
	float answer = (u * h3) + (v * h2) + (w * h1);
	return answer;
}

/* Generates a mesh for the terrain */
Mesh Terrain::generate(const string& height_map)
{
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(height_map.c_str(), &width, &height, &bpp, 3); // 3 channels, rgb

	vertex_count_ = height;

	vector<float> positions;
	vector<float> normals;
	vector<float> texture_coords;
	vector<int> indices;

	for (int row = 0; row < vertex_count_; row++) { // row = z
		for (int col = 0; col < vertex_count_; col++) { // col = x
			positions.push_back((float)col / ((float)vertex_count_ - 1) * size);
			float ht = getHeight(row, col, buffer);
			heights_.push_back(ht);
			positions.push_back(ht);
			positions.push_back((float)row / ((float)vertex_count_ - 1) * size);

			texture_coords.push_back((float)col / ((float)vertex_count_ - 1));
			texture_coords.push_back((float)row / ((float)vertex_count_ - 1));
		}
	}

	for (int row = 0; row < vertex_count_; row++) {
		for (int col = 0; col < vertex_count_; col++) {
			vec3 normal = calculateNormal(row, col, positions);
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
		}
	}

	for (int gz = 0; gz < vertex_count_ - 1; gz++) {
		for (int gx = 0; gx < vertex_count_ - 1; gx++) {
			int top_left = (gz * vertex_count_) + gx;
			int top_right = top_left + 1;
			int bottom_left = ((gz + 1) * vertex_count_) + gx;
			int bottom_right = bottom_left + 1;
			indices.push_back(top_left);
			indices.push_back(bottom_left);
			indices.push_back(top_right);
			indices.push_back(top_right);
			indices.push_back(bottom_left);
			indices.push_back(bottom_right);
		}
	}

	if (buffer)  // if buffer contains data 
		stbi_image_free(buffer);

	return Application::loader.loadToVao(positions, normals, texture_coords, indices);
}

