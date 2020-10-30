#pragma once

#include "Shader.h"

#include <map>

#include "../objects/Entity.h"
#include "../objects/Light.h"
#include "../models/Texture.h"

class DefaultShader : public Shader {
private:
	enum class EUniformVariable {
		TransformationMatrix, ProjectionMatrix, ViewMatrix, InverseViewMatrix,
		LightPosition, LightColor, Reflectivity, ShineDamper
	};

	static const std::string s_vertex_file;
	static const std::string s_fragment_file;

	std::map<EUniformVariable, int> m_locations;

protected:
	virtual void bindAttributes();
	virtual void getAllUniformLocations();

public:
	virtual void setUp();
	void loadViewProjection() const;
	void loadModelMatrix(const Entity& entity) const;
	void loadLight(const Light& light) const;
	void loadMaterial(const ModelTexture& texture) const;
};
