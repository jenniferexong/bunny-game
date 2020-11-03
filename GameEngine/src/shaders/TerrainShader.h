#pragma once

#include <map>

#include "Shader.h"
#include "../objects/Terrain.h"
#include "../objects/Light.h"
#include "../models/Material.h"

class TerrainShader : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, Reflectivity, ShineDamper, SkyColor,
		BaseTexture, RedTexture, GreenTexture, BlueTexture, BlendMap
	};

	static const std::string s_vertex_file;
	static const std::string s_fragment_file;

	std::map<UniformVariable, int> m_locations;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniformPerFrame(const Light& light) const;
	void loadMaterial(const Material& material) const;
	void loadModelMatrix(const Terrain& terrain) const;
	void connectTextureUnits() const;
};
