#pragma once

struct Material {
	float reflectivity = 0;
	float shine_damper = 1;
	bool has_transparency = false;
	bool uses_fake_lighting = false;

	// Default
	Material() {}

	Material(float r, float s, bool transparency, bool fake_lighting) 
		: reflectivity(r), shine_damper(s), has_transparency(transparency), uses_fake_lighting(fake_lighting) {}

	Material(float r, float s) : reflectivity(r), shine_damper(s) {}

	Material(bool transparency, bool fake_lighting) : has_transparency(transparency), uses_fake_lighting(fake_lighting) {}
};
