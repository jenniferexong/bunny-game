
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;

/* Must call this after constructing the shader */
void Shader::setUp(const string& vert_file, const std::string& frag_file)
{
	m_vert_id = loadShader(vert_file, GL_VERTEX_SHADER);
	m_frag_id = loadShader(frag_file, GL_FRAGMENT_SHADER);
	m_program_id = glCreateProgram();
	glAttachShader(m_program_id, m_vert_id);
	glAttachShader(m_program_id, m_frag_id);
	bindAttributes();
	glLinkProgram(m_program_id);
	glValidateProgram(m_program_id);
	getAllUniformLocations();
}

int Shader::loadShader(const string& file_name, int type)
{
	using namespace std;
	ifstream shader_file(file_name);

	ostringstream lines("");
	// Read all the lines in the file
	if (shader_file.is_open()) {
		string line;
		while (getline(shader_file, line)) {
			lines << line << endl;
		}
	}
	shader_file.close(); // close the file
	string text_str = lines.str(); // need this otherwise dangling pointer!

	const GLchar* text = text_str.c_str();
	// Making the shader
	GLint shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &text, nullptr);
	glCompileShader(shader_id);

	// Check the compilation status
	GLint compileStatus;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compileStatus);

	// Printing info log
	if (!compileStatus) {
		cout << lines.str() << endl;
		int info_log_length = 0;
		int chars_written = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		if (info_log_length > 1) {
			vector<char> info_log(info_log_length);
			glGetShaderInfoLog(shader_id, info_log_length, &chars_written, &info_log[0]);
			cout << "Shader: " << &info_log[0] << endl;
		}
		cerr << "Could not compile shader" << endl;

		//exit(EXIT_FAILURE); // failed
	}

	return shader_id;
}

void Shader::bindAttribute(int attribute, const string& variable)
{
	glBindAttribLocation(m_program_id, attribute, variable.c_str());
}

int Shader::getUniformLocation(const string& uniform_name)
{
	return glGetUniformLocation(m_program_id, uniform_name.c_str());
}

void Shader::loadFloat(int location, float value) const
{
	glUniform1f(location, value);
}

void Shader::loadBoolean(int location, bool value) const
{
	// No booleans in glsl, so represent with int of 1 or 0
	int b = value ? 1 : 0;
	glUniform1i(location, b);
}

void Shader::loadVector(int location, glm::vec3 vector) const
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::loadMatrix(int location, glm::mat4 matrix) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::loadVectors(int location, std::vector<glm::vec3>& vectors) const
{
	glUniform3fv(location, vectors.size(), glm::value_ptr(vectors[0]));
}

void Shader::loadMatrices(int location, std::vector<glm::mat4>& matrices) const
{
	glUniformMatrix4fv(location, matrices.size(), GL_FALSE, glm::value_ptr(matrices[0]));
}


void Shader::start()
{
	glUseProgram(m_program_id);
}

void Shader::stop()
{
	glUseProgram(0);
}

Shader::~Shader()
{
	stop();
	glDetachShader(m_program_id, m_vert_id);
	glDetachShader(m_program_id, m_frag_id);
	glDeleteShader(m_vert_id);
	glDeleteShader(m_frag_id);
	glDeleteProgram(m_program_id);
}