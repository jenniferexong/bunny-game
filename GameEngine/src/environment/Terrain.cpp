
#include "Terrain.h"

#include <iostream>
#include <cmath>
#include <stb_image/stb_image.h>

#include "../Application.h"
#include "../Loader.h"
#include "../Location.h"

using std::vector;
using namespace glm;

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
	const string file_path = FilePath::texture_path + height_map + ".png";
	int width, height, bpp; // bits per pixel
	unsigned char* buffer = stbi_load(file_path.c_str(), &width, &height, &bpp, 3); // 3 channels, rgb

	vertex_count_ = height;
	int count = vertex_count_ * vertex_count_;

	vector<int> indices;
	positions_.reserve(count * 3);
	normals_.reserve(count * 3);
	texture_coords_.reserve(count * 2);
	indices.reserve(count);

	for (int row = 0; row < vertex_count_; row++) { // row = z
		for (int col = 0; col < vertex_count_; col++) { // col = x
			positions_.emplace_back((float)col / ((float)vertex_count_ - 1) * size);
			float ht = calculateHeight(row, col, buffer);
			heights_.emplace_back(ht);
			positions_.emplace_back(ht);
			positions_.emplace_back((float)row / ((float)vertex_count_ - 1) * size);

			texture_coords_.emplace_back((float)col / ((float)vertex_count_ - 1));
			texture_coords_.emplace_back((float)row / ((float)vertex_count_ - 1));
		}
	}

	// calculate normals and tangents for each point on the grid
	for (int row = 0; row < vertex_count_; row++) {
		for (int col = 0; col < vertex_count_; col++) {
			vec3 normal = calculateAverageValue(row, col, &Terrain::calculateNormal);
			normals_.emplace_back(normal.x);
			normals_.emplace_back(normal.y);
			normals_.emplace_back(normal.z);

			vec3 tangent = calculateAverageValue(row, col, &Terrain::calculateTangent);
			tangents_.emplace_back(tangent.x);
			tangents_.emplace_back(tangent.y);
			tangents_.emplace_back(tangent.z);
		}
	}

	for (int row = 0; row < vertex_count_ - 1; row++) {
		for (int col = 0; col < vertex_count_ - 1; col++) {
			int top_left = (row * vertex_count_) + col;
			int top_right = top_left + 1;
			int bottom_left = ((row + 1) * vertex_count_) + col;
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

	return app->loader->loadToVao(positions_, normals_, texture_coords_, tangents_, indices);
}

float Terrain::calculateHeight(int row, int col, const unsigned char* buffer)
{
	if (invalidVertex(row, col))
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
glm::vec3 Terrain::calculateAverageValue(int row, int col,
		void(Terrain::* calculate_function)(glm::ivec2 v0, glm::ivec2 v1, glm::ivec2 v2, std::vector<glm::vec3>& values))
{
	vector<vec3> neighbouring;

	ivec2 origin = ivec2(row, col);
	ivec2 north = ivec2(row - 1, col);
	ivec2 north_east = ivec2(row - 1, col + 1);
	ivec2 east = ivec2(row, col + 1);
	ivec2 south = ivec2(row + 1, col);
	ivec2 south_west = ivec2(row + 1, col - 1);
	ivec2 west = ivec2(row, col - 1);

	// each vertex has a maximum of 6 neighbouring triangles
	// NNE triangle
	(this->*calculate_function)(north, origin, north_east, neighbouring);
	// ENE triangle
	(this->*calculate_function)(east, north_east, origin, neighbouring);
	// SE triangle
	(this->*calculate_function)(origin, south, east, neighbouring);
	// SSW triangle
	(this->*calculate_function)(south, origin, south_west, neighbouring);
	// WSW triangle
	(this->*calculate_function)(west, south_west, origin, neighbouring);
	// NW triangle
	(this->*calculate_function)(origin, north, west, neighbouring);

	vec3 total(0.f);
	for (auto t : neighbouring)
		total += t;

	// average
	vec3 value = total / (float)neighbouring.size();
	return normalize(value);
}

/* Calculates the normal of the triangle where each vertex is given in row and col of the grid */
void Terrain::calculateNormal(ivec2 v0 , ivec2 v1, ivec2 v2, vector<vec3>& normals)
{
	if (invalidVertex(v0.x, v0.y) || invalidVertex(v1.x, v1.y) || invalidVertex(v2.x, v2.y))
		return;

	vec3 p0 = getPosition(v0.x, v0.y);
	vec3 p1 = getPosition(v1.x, v1.y);
	vec3 p2 = getPosition(v2.x, v2.y);

	vec3 d1 = p1 - p0;
	vec3 d2 = p2 - p0;
	vec3 normal = cross(d1, d2);
	normals.push_back(normalize(normal));
}

/* Calculates the tangent of the triangle where each vertex is given in row and col of the grid */
void Terrain::calculateTangent(ivec2 v0 , ivec2 v1, ivec2 v2, vector<vec3>& tangents)
{
	if (invalidVertex(v0.x, v0.y) || invalidVertex(v1.x, v1.y) || invalidVertex(v2.x, v2.y))
		return;

	vec3 p0 = getPosition(v0.x, v0.y);
	vec3 p1 = getPosition(v1.x, v1.y);
	vec3 p2 = getPosition(v2.x, v2.y);
	vec2 t0 = getTextureCoordinate(v0.x, v0.y);
	vec2 t1 = getTextureCoordinate(v1.x, v1.y);
	vec2 t2 = getTextureCoordinate(v2.x, v2.y);

	vec3 delta_p1 = p1 - p0;
	vec3 delta_p2 = p2 - p0;
	vec2 delta_t1 = t1 - t0;
	vec2 delta_t2 = t2 - t0;

	float r = 1.f / (delta_t1.x * delta_t2.y - delta_t1.y * delta_t2.x);
	vec3 tangent = (delta_p1 * delta_t2.y - delta_p2 * delta_t1.y) * r;
	tangents.push_back(tangent);
}

bool Terrain::invalidVertex(int row, int col)
{
	return row < 0 || row >= vertex_count_ || col < 0 || col >= vertex_count_;
}

vec2 Terrain::getTextureCoordinate(int row, int col) const
{
	int index = 2 * (row * vertex_count_ + col); // 3 channels
	return vec2(texture_coords_.at(index), texture_coords_.at(index + 1));
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

