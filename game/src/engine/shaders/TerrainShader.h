#pragma once

#include <glm/vec4.hpp>

#include <unordered_map>

#include "Shader.h"

struct Material;
class Terrain;
class Environment;

class TerrainShader final : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, LightCount, MaxLights, Attenuation,
		Reflectivity, ShineDamper, FogColor, SunStrength,
		BaseTexture, RedTexture, GreenTexture, BlueTexture, BlendMap, NormalMap,
		ClippingPlane
	};

	static constexpr char vertex_file[] = "terrain-vert";
	static constexpr char fragment_file[] = "terrain-frag";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

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
