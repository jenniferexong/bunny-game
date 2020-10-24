#pragma once

#include <string>
#include <vector>

// Abstract class for generic shaders
class Shader {

private:
	int m_program_id;
	int m_vert_id;
	int m_frag_id;

	static int loadShader(const std::string& file_name, int type);

protected:
	virtual void bindAttributes() = 0; // need to override

	void bindAttribute(int attribute, const std::string& variable);

public:
	Shader(const std::string& vert_file, const std::string& frag_file); // constructor

	void start();
	void stop();
	void destroy();
};
