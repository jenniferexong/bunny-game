#pragma once

#include <string>
#include <vector>

#include "../Maths.h"
#include "../AttributeLocation.h"

// Abstract class for generic shaders
class Shader {

private:
	int m_program_id;
	int m_vert_id;
	int m_frag_id;

	static int loadShader(const std::string& file_name, int type);

protected:
	virtual void bindAttributes() = 0; // need to override
	virtual void getAllUniformLocations() = 0;

	void bindAttribute(int attribute, const std::string& variable);
	int getUniformLocation(const std::string& uniform_name);

	// Loading uniform variables
	void loadFloat(int location, float value) const;
	void loadBoolean(int location, bool value) const;
	void loadInt(int location, int value) const;
	void loadVector(int location, glm::vec3 value) const;
	void loadMatrix(int location, glm::mat4 matrix) const;
	void loadVectors(int location, std::vector<glm::vec3>& vectors) const;
	void loadMatrices(int location, std::vector<glm::mat4>& matrices) const;

public:
	//Shader(const std::string& vert_file, const std::string& frag_file); // constructor
	Shader() : m_program_id(0), m_vert_id(0), m_frag_id(0) {}
	~Shader();

	void setUp(const std::string& vert_file, const std::string& frag_file);
	virtual void setUp() = 0;

	void start();
	void stop();
};
