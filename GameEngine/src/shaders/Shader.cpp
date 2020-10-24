
#include <GL/glew.h>

#include "Shader.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;

int Shader::loadShader(const string& file_name, int type) {
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

void Shader::bindAttribute(int attribute, const std::string& variable) {
	glBindAttribLocation(m_program_id, attribute, variable.c_str());
}

Shader::Shader(const std::string& vert_file, const std::string& frag_file) {
	m_vert_id = loadShader(vert_file, GL_VERTEX_SHADER);
	m_frag_id = loadShader(frag_file, GL_FRAGMENT_SHADER);
	m_program_id = glCreateProgram();
	glAttachShader(m_program_id, m_vert_id);
	glAttachShader(m_program_id, m_frag_id);
	glLinkProgram(m_program_id);
	glValidateProgram(m_program_id);
}

void Shader::start() {
	glUseProgram(m_program_id);

	// Upload uniform variables
	glUniform1i(glGetUniformLocation(m_program_id, "textureSampler"), 0);
}

void Shader::stop() {
	glUseProgram(0);
}

void Shader::destroy() {
	stop();
	glDetachShader(m_program_id, m_vert_id);
	glDetachShader(m_program_id, m_frag_id);
	glDeleteShader(m_vert_id);
	glDeleteShader(m_frag_id);
	glDeleteProgram(m_program_id);
}