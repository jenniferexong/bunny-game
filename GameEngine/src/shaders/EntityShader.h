#pragma once

#include "Shader.h"

#include <map>

#include "../environment/Entity.h"
#include "../models/Material.h"
#include "../scene/Scene.h"

class EntityShader final : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, LightCount, MaxLights, Attenuation,
		Reflectivity, ShineDamper, FakeLighting, FogColor, SunStrength,
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
	void loadModelMatrix(const Entity& entity) const;
	void loadMaterial(const Material& material) const;
};
