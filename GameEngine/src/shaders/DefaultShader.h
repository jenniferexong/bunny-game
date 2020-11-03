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
		LightPosition, LightColor, Reflectivity, ShineDamper, FakeLighting, SkyColor
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
	void loadModelMatrix(const Entity& entity) const;
	void loadMaterial(const Material& material) const;

	/*
	void loadViewProjection() const;
	void loadLight(const Light& light) const;
	*/
};
