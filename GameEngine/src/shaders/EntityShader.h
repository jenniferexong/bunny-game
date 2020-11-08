#pragma once

#include "Shader.h"

#include <map>

#include "../objects/Entity.h"
#include "../objects/Light.h"
#include "../models/Material.h"

class DefaultShader final : public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, LightCount, MaxLights,
		Reflectivity, ShineDamper, FakeLighting, SkyColor
	};

	static const std::string vertex_file;
	static const std::string fragment_file;

	std::map<UniformVariable, int> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniformPerFrame(const std::vector<Light>& lights) const;
	void loadModelMatrix(const Entity& entity) const;
	void loadMaterial(const Material& material) const;
};
