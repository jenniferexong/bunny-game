#pragma once

#include "Shader.h"

#include <unordered_map>

struct Material;
class Entity;
class Environment;

class EntityShader final : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, LightCount, MaxLights, Attenuation,
		Reflectivity, ShineDamper, FakeLighting, FogColor, SunStrength,
		ClippingPlane, DiffuseMap, GlowMap, EnvironmentMap
	};

	static constexpr char name[] = "EntityShader";
	static constexpr char vertex_file[] = "entity-vert";
	static constexpr char fragment_file[] = "entity-frag";

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void connectTextureUnits();
	void loadUniformPerFrame(
		const Environment& environment,
		glm::vec4 clipping_plane
	) const;
	void loadModelMatrix(const Entity& entity) const;
	void loadMaterial(const Material& material) const;
};
