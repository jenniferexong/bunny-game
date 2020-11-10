﻿#pragma once

#include <unordered_map>

#include "Shader.h"

class SkyboxShader final: public Shader {
	
private:
	enum class UniformVariable {
		ProjectionMatrix, ViewMatrix
	};

	static const std::string vertex_file;
	static const std::string fragment_file;

	std::unordered_map<UniformVariable, int> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniforms() const;
};
