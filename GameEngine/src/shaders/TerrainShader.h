#pragma once

#include <map>

#include "Shader.h"

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
};