
#include "Terrain.h"

#include <iostream>
#include <cmath>
#include <stb_image/stb_image.h>

#include "../Application.h"

using std::vector;
using namespace glm;

const float Terrain::size = 510.f;
const float Terrain::max_height = 35.f;
const float Terrain::max_pixel_color = 255 * 3;

Terrain::Terrain(int grid_x, int grid_z, TerrainTexture texture, const string& height_map)
	: x_(float(grid_x) * size), z_(float(grid_z) * size), texture_(std::move(texture))
{
	mesh_ = generate(height_map);
}

template <class Value>
Value Terrain::barycentric(vec3 point, vec3 p1, vec3 p2, vec3 p3, Value v1, Value v2, Value v3) const
{
	const float area = 0.5f;
	float u = glm::length(glm::cross(p1 - point, p2 - point)) / 2.f / area;
	float v = glm::length(glm::cross(p1 - point, p3 - point)) / 2.f / area;
	float w = 1.f - u - v;
	Value answer = (u * v3) + (v * v2) + (w * v1);
	return answer;
}

vec3 Terrain::getNormalOfTerrain(float world_x, float world_z) const
{
	float terrain_x = world_x - x_; 
	float terrain_z = world_z - z_;

	float grid_size = size / float(vertex_count_ - 1);
	int grid_x = (int) floor(terrain_x / grid_size);
	int grid_z = (int) floor(terrain_z / grid_size);

	// check if actually a valid grid on the terrain
	if (grid_x >= vertex_count_ - 1 || grid_z >= vertex_count_ - 1 || grid_x < 0 || grid_z < 0)
		return vec3(0, 1.f, 0);

	// get the x and z amount in its grid, in range [0, 1]
	float x = fmod(terrain_x, grid_size) / grid_size;
	float z = fmod(terrain_z, grid_size) / grid_size;

	// check which triangle the point in is, then use barycentric interpolation to find the normal
	vec3 answer(0.f);
	if (x <= (1 - z)) { // left triangle
		answer = barycentric<vec3>(vec3(x, 0, z), vec3(0, 0, 0), vec3(0, 0, 1), vec3(1, 0, 0),
		getNormal(grid_z, grid_x), getNormal(grid_z + 1, grid_x), getNormal(grid_z, grid_x + 1));
	}
	else if (x > (1 - z)) { // right triangle
		answer = barycentric<vec3>(vec3(x, 0, z), vec3(1, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1),
		getNormal(grid_z + 1, grid_x + 1), getNormal(grid_z, grid_x + 1), getNormal(grid_z + 1, grid_x));
	}
	return normalize(answer);
}


float Terrain::getHeightOfTerrain(float world_x, float world_z) const
{
	float terrain_x = world_x - x_;
	float terrain_z = world_z - z_;

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
		answer = barycentric<float>(vec3(x, 0, z), vec3(0, 0, 0), vec3(0, 0, 1), vec3(1, 0, 0),
			getHeight(grid_z, grid_x), getHeight(grid_z + 1, grid_x), getHeight(grid_z, grid_x + 1));
	}
	else { // right triangle
		answer = barycentric<float>(vec3(x, 0, z), vec3(1, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1),
			getHeight(grid_z + 1, grid_x + 1), getHeight(grid_z, grid_x + 1), getHeight(grid_z + 1, grid_x));
	}
	return answer;
}

/* Generates a mesh for the terrain */
Mesh Terrain::generate(const string& height_map)
{
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(height_map.c_str(), &width, &height, &bpp, 3); // 3 channels, rgb

	vertex_count_ = height;
	int count = vertex_count_ * vertex_count_;

	vector<float> texture_coords;
	vector<int> indices;
	positions_.reserve(count * 3);
	normals_.reserve(count * 3);
	texture_coords.reserve(count * 2);
	indices.reserve(count);

	for (int row = 0; row < vertex_count_; row++) { // row = z
		for (int col = 0; col < vertex_count_; col++) { // col = x
			positions_.emplace_back((float)col / ((float)vertex_count_ - 1) * size);
			float ht = calculateHeight(row, col, buffer);
			heights_.emplace_back(ht);
			positions_.emplace_back(ht);
			positions_.emplace_back((float)row / ((float)vertex_count_ - 1) * size);

			texture_coords.emplace_back((float)col / ((float)vertex_count_ - 1));
			texture_coords.emplace_back((float)row / ((float)vertex_count_ - 1));
		}
	}

	for (int row = 0; row < vertex_count_; row++) {
		for (int col = 0; col < vertex_count_; col++) {
			vec3 normal = calculateNormal(row, col);
			normals_.emplace_back(normal.x);
			normals_.emplace_back(normal.y);
			normals_.emplace_back(normal.z);
		}
	}

	for (int gz = 0; gz < vertex_count_ - 1; gz++) {
		for (int gx = 0; gx < vertex_count_ - 1; gx++) {
			int top_left = (gz * vertex_count_) + gx;
			int top_right = top_left + 1;
			int bottom_left = ((gz + 1) * vertex_count_) + gx;
			int bottom_right = bottom_left + 1;
			indices.emplace_back(top_left);
			indices.emplace_back(bottom_left);
			indices.emplace_back(top_right);
			indices.emplace_back(top_right);
			indices.emplace_back(bottom_left);
			indices.emplace_back(bottom_right);
		}
	}

	if (buffer)
		stbi_image_free(buffer);

	return Application::loader.loadToVao(positions_, normals_, texture_coords, indices);
}

float Terrain::calculateHeight(int row, int col, const unsigned char* buffer)
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

/* Calculates the normal of the vertex based on the normal of the 4 neighbouring faces */
vec3 Terrain::calculateNormal(int row, int col) const 
{
	vec3 x_offset = vec3(1, 0, 0);
	vec3 z_offset = vec3(0, 0, 1);
	vec3 position = getPosition(row, col);
	vec3 north_position = row - 1 < 0 ? position - z_offset : getPosition(row - 1, col);
	vec3 east_position = col + 1 >= vertex_count_ ? position + x_offset : getPosition(row, col + 1);
	vec3 south_position = row + 1 >= vertex_count_ ? position + z_offset : getPosition(row + 1, col);
	vec3 west_position = col - 1 < 0 ? position - x_offset : getPosition(row, col - 1);

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

vec3 Terrain::getPosition(int row, int col) const
{
	int index = 3 * (row * vertex_count_ + col); // 3 channels
	return vec3(positions_.at(index), positions_.at(index + 1), positions_.at(index + 2));
}

vec3 Terrain::getNormal(int row, int col) const
{
	int index = 3 * (row * vertex_count_ + col); // 3 channels
	return vec3(normals_.at(index), normals_.at(index + 1), normals_.at(index + 2));
}

float Terrain::getHeight(int x, int z) const
{
	int index = x * vertex_count_ + z;
	return heights_.at(index);
}

