#pragma once

#include "Shader.h"

class DefaultShader : public Shader {
private:
	static const std::string s_vertex_file;
	static const std::string s_fragment_file;

protected:
	virtual void bindAttributes();

public:
	DefaultShader() : Shader(s_vertex_file, s_fragment_file) { // Calling base class constructor
		bindAttributes();
	}
};
