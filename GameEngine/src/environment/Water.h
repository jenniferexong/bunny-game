#pragma once
#include <glm/vec4.hpp>

// a square tile of water
class Water {
private:
	float height_; // height in the world
	float centre_x, centre_z;

public:
	static const float tile_size;

	Water(float x, float z, float height) : centre_x(x), centre_z(z), height_(height) {}

	float getX() const { return centre_x; }
	float getZ() const { return centre_z; }
	float getHeight() const { return height_; }

	static glm::vec4 getReflectionPlane();
	static glm::vec4 getRefractionPlane();
};
