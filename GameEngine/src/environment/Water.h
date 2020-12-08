#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// a square tile of water
class Water {
private:
	float centre_x, centre_z;

public:
	static constexpr float height = -18.f; // height in the world
	static constexpr float tile_size = 42.5f;

	Water(float x, float z) : centre_x(x), centre_z(z) {}
	~Water() = default;

	float getX() const { return centre_x; }
	float getZ() const { return centre_z; }

	bool containsPoint(glm::vec3 point) const;

	static glm::vec4 getReflectionPlane();
	static glm::vec4 getRefractionPlane();
};
