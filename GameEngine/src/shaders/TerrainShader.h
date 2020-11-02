#pragma once

#include <map>

#include "Shader.h"
#include "../objects/Terrain.h"
#include "../objects/Light.h"
#include "../models/Material.h"

class TerrainShader : public Shader {
private:
	enum class EUniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, Reflectivity, ShineDamper, SkyColor,
		BaseTexture, RedTexture, GreenTexture, BlueTexture, BlendMap
	};

	static const std::string s_vertex_file;
	static const std::string s_fragment_file;

	std::map<EUniformVariable, int> m_locations;

protected:
	virtual void bindAttributes();
	virtual void getAllUniformLocations();

public:
	virtual void setUp();
	void loadUniformPerFrame(const Light& light) const;
	void loadMaterial(const Material& material) const;
	void loadModelMatrix(const Terrain& terrain) const;
	void connectTextureUnits() const;
};
