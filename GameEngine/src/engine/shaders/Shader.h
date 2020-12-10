#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

using std::string;

// Abstract class for generic shaders
class Shader {

private:
	int program_id_ = -1;
	int vert_id_ = -1;
	int frag_id_ = -1;

	static int loadShader(const string& file_name, int type);

protected:
	virtual void bindAttributes() = 0; // need to override
	virtual void getAllUniformLocations() = 0;

	void bindAttribute(int attribute, const std::string& variable);
	int getUniformLocation(const std::string& uniform_name);

	// Loading uniform variables
	void loadFloat(int location, float value) const;
	void loadBoolean(int location, bool value) const;
	void loadInt(int location, int value) const;
	void loadVector(int location, glm::vec2 vector) const;
	void loadVector(int location, glm::vec3 vector) const;
	void loadVector(int location, glm::vec4 vector) const;
	void loadMatrix(int location, glm::mat4 matrix) const;
	void loadVectors(int location, const std::vector<glm::vec3>& vectors) const;
	void loadMatrices(int location, const std::vector<glm::mat4>& matrices) const;

public:
	Shader() : program_id_(0), vert_id_(0), frag_id_(0) {}
	Shader(const Shader& other) = default;
	virtual ~Shader();

	void setUp(const string& vert_file, const string& frag_file);
	virtual void setUp() = 0;

	void start() const;
	void stop() const;
	void print() const;
};
