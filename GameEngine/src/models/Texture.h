#pragma once

#include <memory>

#include "Material.h"

class Texture;
class ModelTexture;
class TerrainTexture;
class TerrainTexturePack;

class Texture {
private:
	int texture_id_;

public:
	Texture(int id) : texture_id_(id) {}

	int getId() const { return texture_id_; }

	Texture() : texture_id_(0) {}
};

/* A texture for a model */
class ModelTexture {
private:
	Texture texture_;
	Material material_ = Material();

public:
	ModelTexture(Texture texture, Material material) : texture_(texture), material_(material) {}

	int getTextureId() const { return texture_.getId(); }
	Material getMaterial() const { return material_; }
};

class TerrainTexture {
private:
	std::shared_ptr<TerrainTexturePack> texture_pack_;
	Texture blend_map_;
	Material material_ = Material();

public:
	TerrainTexture() : texture_pack_(nullptr) {}

	TerrainTexture(std::shared_ptr<TerrainTexturePack> texture_pack, Texture blend_map)
		: texture_pack_(std::move(texture_pack)), blend_map_(blend_map) {}

	Texture getBlendMap() const { return blend_map_; }
	std::shared_ptr<TerrainTexturePack> getTexturePack() const { return texture_pack_; }

	Material getMaterial() const { return material_; }
};

class TerrainTexturePack {
private:
	Texture base_, red_, green_, blue_;

public:
	TerrainTexturePack(Texture base, Texture red, Texture green, Texture blue)
		: base_(base), red_(red), green_(green), blue_(blue) {}

	Texture getBase() const { return base_; }
	Texture getRed() const { return red_; }
	Texture getGreen() const { return green_; }
	Texture getBlue() const { return blue_; }
};




