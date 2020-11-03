#pragma once

#include "../models/Mesh.h"
#include "../models/Texture.h"

class Terrain {
private:
	static const float size;
	static const int vertex_count;
	
	float _x, _z;
	Mesh _mesh;
	TerrainTexture _texture;

	Mesh generate();

public:
	Terrain() : _x(0), _z(0) {}
	Terrain(int grid_x, int grid_z, TerrainTexture texture);

	float getX() const { return _x; }
	float getZ() const { return _z; }
	Mesh getMesh() const { return _mesh; }
	TerrainTexture getTexture() const { return _texture; }
};
