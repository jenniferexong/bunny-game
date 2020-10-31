#pragma once

#include <string>

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

/* A texture for a model */
class ModelTexture {
private:
	int m_texture_id;
	float m_reflectivity = 0;
	float m_shine_damper = 1;
	bool m_hasTransparency = false;
	bool m_uses_fake_lighting = false;

public:
	ModelTexture() {}
	ModelTexture(int id) : m_texture_id(id) {}
	inline int getId() const { return m_texture_id; }
	inline float getReflectivity() const { return m_reflectivity; }
	inline float getShineDamper() const { return m_shine_damper; }
	inline bool hasTransparency() const { return m_hasTransparency; }
	inline bool usesFakeLighting() const { return m_uses_fake_lighting; }

	inline void setReflectivity(float reflectivity) { m_reflectivity = reflectivity; }
	inline void setShineDamper(float shine_damper) { m_shine_damper = shine_damper; }
	inline void setTransparent(bool has_transparency) { m_hasTransparency = has_transparency; }
	inline void setFakeLighting(bool uses_fake_lighting) { m_uses_fake_lighting = uses_fake_lighting; }
};
