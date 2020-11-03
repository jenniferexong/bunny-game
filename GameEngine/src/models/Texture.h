#pragma once

#include <memory>

#include "Material.h"

class Texture;
class ModelTexture;
class TerrainTexture;
class TerrainTexturePack;

class Texture {
private:
	int m_texture_id;

public:
	Texture(int id) : m_texture_id(id) {}

	int getId() const { return m_texture_id; }

	Texture() : m_texture_id(0) {}
};

/* A texture for a model */
class ModelTexture {
private:
	Texture m_texture;
	Material m_material = Material();

public:
	ModelTexture(Texture texture, Material material) : m_texture(texture), m_material(material) {}

	int getTextureId() const { return m_texture.getId(); }
	Material getMaterial() const { return m_material; }
};

class TerrainTexture {
private:
	std::shared_ptr<TerrainTexturePack> m_texture_pack;
	Texture m_blend_map;
	Material m_material = Material();

public:
	TerrainTexture() : m_texture_pack(nullptr) {}

	TerrainTexture(std::shared_ptr<TerrainTexturePack> texture_pack, Texture blend_map)
		: m_texture_pack(std::move(texture_pack)), m_blend_map(blend_map) {}

	Texture getBlendMap() const { return m_blend_map; }
	std::shared_ptr<TerrainTexturePack> getTexturePack() const { return m_texture_pack; }

	Material getMaterial() const { return m_material; }
};

class TerrainTexturePack {
private:
	Texture m_base, m_red, m_green, m_blue;

public:
	TerrainTexturePack(Texture base, Texture red, Texture green, Texture blue)
		: m_base(base), m_red(red), m_green(green), m_blue(blue) {}

	Texture getBase() const { return m_base; }
	Texture getRed() const { return m_red; }
	Texture getGreen() const { return m_green; }
	Texture getBlue() const { return m_blue; }
};




