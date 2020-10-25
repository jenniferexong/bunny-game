#pragma once

#include "Shader.h"

#include <map>

class Entity;

class DefaultShader : public Shader {
private:
	enum class EUniformVariable {
		TransformationMatrix, ProjectionMatrix
	};

	static const std::string s_vertex_file;
	static const std::string s_fragment_file;

	std::map<EUniformVariable, int> m_locations;

protected:
	virtual void bindAttributes();
	virtual void getAllUniformLocations();
	virtual void loadAllUniforms(const glm::mat4 projection, const Entity& entity) const;

public:
	virtual void setUp();
};
