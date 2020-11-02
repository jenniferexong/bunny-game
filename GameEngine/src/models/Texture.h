#pragma once

//#include <string>
#include <memory>

#include "Material.h"

/*
class Texture {
private:
	unsigned int m_id;
	std::string m_file_name;
	unsigned char* m_local_buffer; // storing the pixel data
	int m_width, m_height, m_bpp; // bits per pixel

public:
	Texture(const std::string& file_name);

	inline int getId() const { return m_id; }
	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
};
*/
class ModelTexture;
class TerrainTexture;
class TerrainTexturePack;
class Texture;

class Texture {
private:
	int m_texture_id;

public:
	Texture(int id) : m_texture_id(id) {}

	inline int getId() const { return m_texture_id; }

	Texture() : m_texture_id(0) {}
};

/* A texture for a model */
class ModelTexture {
private:
	Texture m_texture;
	Material m_material = Material();

public:
	ModelTexture(Texture texture, Material material) : m_texture(texture), m_material(material) {}

	inline int getTextureId() const { return m_texture.getId(); }
	inline Material getMaterial() const { return m_material; }
};

class TerrainTexture {
private:
	std::shared_ptr<TerrainTexturePack> m_texture_pack;
	Texture m_blend_map;
	Material m_material = Material();

public:
	TerrainTexture() : m_texture_pack(nullptr) {}

	TerrainTexture(std::shared_ptr<TerrainTexturePack> texture_pack, Texture blend_map)
		: m_texture_pack(texture_pack), m_blend_map(blend_map) {}

	inline Texture getBlendMap() const { return m_blend_map; }
	inline std::shared_ptr<TerrainTexturePack> getTexturePack() const { return m_texture_pack; }

	inline Material getMaterial() const { return m_material; }
};

class TerrainTexturePack {
private:
	Texture m_base, m_red, m_green, m_blue;

public:
	TerrainTexturePack(Texture base, Texture red, Texture green, Texture blue)
		: m_base(base), m_red(red), m_green(green), m_blue(blue) {}

	inline Texture getBase() const { return m_base; }
	inline Texture getRed() const { return m_red; }
	inline Texture getGreen() const { return m_green; }
	inline Texture getBlue() const { return m_blue; }
};


