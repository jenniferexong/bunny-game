#pragma once

#include <map>

#include "Shader.h"

#include "../environment/Terrain.h"
#include "../models/Material.h"

#include "../scene/Scene.h"

class TerrainShader final : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, LightCount, MaxLights, Attenuation,
		Reflectivity, ShineDamper, FogColor, SunStrength,
		BaseTexture, RedTexture, GreenTexture, BlueTexture, BlendMap, NormalMap,
		ClippingPlane
	};

	static const std::string vertex_file;
	static const std::string fragment_file;

	std::map<UniformVariable, int> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniformPerFrame(const Environment& environment, glm::vec4 clipping_plane) const;
	void loadMaterial(const Material& material) const;
	void loadModelMatrix(const Terrain& terrain) const;
	void connectTextureUnits() const;
};
