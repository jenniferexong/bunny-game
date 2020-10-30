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

public:
	ModelTexture(int id) : m_texture_id(id) {}
	inline int getId() const { return m_texture_id; }
	inline float getReflectivity() const {return m_reflectivity; }
	inline float getShineDamper() const {return m_shine_damper; }

	inline void setReflectivity(float reflectivity) { m_reflectivity = reflectivity; }
	inline void setShineDamper(float shine_damper) { m_shine_damper = shine_damper; }
};
