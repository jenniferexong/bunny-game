#pragma once

#include <map>

#include "Shader.h"
#include "../objects/Terrain.h"
#include "../objects/Light.h"

class TerrainShader : public Shader {
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
	void loadModelMatrix(const Terrain& terrain);
	void loadLight(const Light& light) const;
	void loadMaterial(const ModelTexture& texture) const;
};
