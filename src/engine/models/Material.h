#pragma once
#include <string>
#include <filesystem>

#include "../Application.h"

struct Material {
	float reflectivity = 0;
	float shine_damper = 1;
	bool has_transparency = false;
	bool uses_fake_lighting = false;

	int glow_map_ = -1;

	// Default
	Material() = default; 
	~Material() = default; 

	Material(float r, float s, bool transparency, bool fake_lighting): 
		reflectivity(r), 
		shine_damper(s),
		has_transparency(transparency),
		uses_fake_lighting(fake_lighting) 
	{
		setGlowMap("black.png");
	}

	Material(float r, float s) : reflectivity(r), shine_damper(s) 
	{
		setGlowMap("black.png");
	}

	Material(bool transparency, bool fake_lighting): 
		has_transparency(transparency),
		uses_fake_lighting(fake_lighting) {}

	void setGlowMap(const std::filesystem::path& texture_file)
	{
		glow_map_ = app->loader->loadTexture(texture_file);
	}

	bool hasGlowMap()
	{
		return glow_map_ > -1;
	}
};
