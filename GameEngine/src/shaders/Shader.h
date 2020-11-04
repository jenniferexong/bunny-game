#pragma once

#include <string>
#include <vector>

#include "../Maths.h"

// Abstract class for generic shaders
class Shader {

private:
	int program_id_;
	int vert_id_;
	int frag_id_;

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
	void loadVector(int location, vec3 vector) const;
	void loadMatrix(int location, glm::mat4 matrix) const;
	void loadVectors(int location, std::vector<vec3>& vectors) const;
	void loadMatrices(int location, std::vector<glm::mat4>& matrices) const;

public:
	//Shader(const std::string& vert_file, const std::string& frag_file); // constructor
	Shader() : program_id_(0), vert_id_(0), frag_id_(0) {}
	virtual ~Shader();

	void setUp(const std::string& vert_file, const std::string& frag_file);
	virtual void setUp() = 0;

	void start() const;
	void stop() const;
};
