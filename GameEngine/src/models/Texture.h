#pragma once

#include <memory>

#include "Material.h"

class Texture;
class ModelTexture;
class TerrainTexture;
class TerrainTexturePack;

class Texture {
private:
	int _texture_id;

public:
	Texture(int id) : _texture_id(id) {}

	int getId() const { return _texture_id; }

	Texture() : _texture_id(0) {}
};

/* A texture for a model */
class ModelTexture {
private:
	Texture _texture;
	Material _material = Material();

public:
	ModelTexture(Texture texture, Material material) : _texture(texture), _material(material) {}

	int getTextureId() const { return _texture.getId(); }
	Material getMaterial() const { return _material; }
};

class TerrainTexture {
private:
	std::shared_ptr<TerrainTexturePack> m_texture_pack;
	Texture _blend_map;
	Material _material = Material();

public:
	TerrainTexture() : m_texture_pack(nullptr) {}

	TerrainTexture(std::shared_ptr<TerrainTexturePack> texture_pack, Texture blend_map)
		: m_texture_pack(std::move(texture_pack)), _blend_map(blend_map) {}

	Texture getBlendMap() const { return _blend_map; }
	std::shared_ptr<TerrainTexturePack> getTexturePack() const { return m_texture_pack; }

	Material getMaterial() const { return _material; }
};

class TerrainTexturePack {
private:
	Texture _base, _red, _green, _blue;

public:
	TerrainTexturePack(Texture base, Texture red, Texture green, Texture blue)
		: _base(base), _red(red), _green(green), _blue(blue) {}

	Texture getBase() const { return _base; }
	Texture getRed() const { return _red; }
	Texture getGreen() const { return _green; }
	Texture getBlue() const { return _blue; }
};




