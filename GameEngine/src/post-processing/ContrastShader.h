#pragma once

#include <unordered_map>

#include "../shaders/Shader.h"

class ContrastShader final : public Shader {

	enum class UniformVariable {
	};

	static const std::string vertex_file;
	static const std::string fragment_file;

	std::unordered_map<UniformVariable, int> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
};
