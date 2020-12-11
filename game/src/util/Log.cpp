#include <GL/glew.h>

#include "Log.h"

#include <iostream>
#include <sstream>
#include <glm/gtc/matrix_access.hpp>

using std::string;
using std::cout;
using std::endl;


void Error::file(const std::string& type, const std::string& path)
{
	std::stringstream message;
	message << "error reading file: " << type << ", " << path;
	Error::exit(message.str());
}

void Error::gl_check(const string& location)
{
	GLenum err;
	std::stringstream message;

	//const GLubyte* error; 
	if((err = glGetError()) != GL_NO_ERROR){
		message << "GL ERROR IN: " << location << endl;
 		message << "\t- " << gluErrorString(err);
		while((err = glGetError()) != GL_NO_ERROR)
 			message << endl << '\t' << gluErrorString(err);
		Error::exit(message.str());
	}  
}

void Error::exit(const string& message)
{
	cout << endl << line << endl;
	cout << message << endl;
	cout << line << endl << endl;
	std::exit(EXIT_FAILURE);
}

void Log::destroy(message object, int id)
{
	if (!Log::print)
		return;

	cout << "destroyed: " << object << ", id: " << id << endl;
}

void Log::init(message object, bool success) 
{
	if (!Log::print)
		return;
	string output = "init: ";
	output.append(object);
	if (success)
		output.append(", success!");
	else
		output.append(", begin");
	cout << output << endl;
}

void Log::val(message message, float value)
{
	if (!Log::print)
		return;
	cout << message << ": " << value << endl;
}

void Log::val(message message, int value)
{
	if (!Log::print)
		return;
	cout << message << ": " << value << endl;
}

void Log::s(int num)
{
	if (!Log::print)
		return;

	cout << "[ " <<  num  << " ]" << endl;
}

void Log::s(message message)
{
	if (!Log::print)
		return;
	cout << '\t' << message << endl;
}

void Log::texture(message texture_name, int id)
{
	if (!Log::print)
		return;
	printf("\tloaded texture: %s, %d\n", texture_name.c_str(), id);
}

void Log::fbo(message attachment, int id)
{
	if (!Log::print)
		return;

	printf("\tcreated fbo (%s): %d\n", attachment.c_str(), id);
}

void Log::matrix(message message, glm::mat4 matrix)
{
	if (!Log::print)
		return;
	using glm::vec4;
	vec4 r1 = glm::row(matrix, 0);
	vec4 r2 = glm::row(matrix, 1);
	vec4 r3 = glm::row(matrix, 2);
	vec4 r4 = glm::row(matrix, 3);

	cout << message << endl;
	vector(r1);
	vector(r2);
	vector(r3);
	vector(r4);
}

void Log::vector(message message, glm::vec4 vector)
{
	if (!Log::print)
		return;
	cout << message << endl;
	Log::vector(vector);
}

void Log::vector(message message, glm::vec3 vector)
{
	if (!Log::print)
		return;
	cout << "-------------------" << endl;
	cout << message << endl;
	Log::vector(vector);
}

void Log::vector(glm::vec3 vector)
{
	if (!Log::print)
		return;
	printf("%.2f, %.2f, %.2f\n", vector.x, vector.y, vector.z);
}

void Log::vector(glm::vec4 vector)
{
	if (!Log::print)
		return;
	printf("%.2f, %.2f, %.2f, %.2f\n", vector.x, vector.y, vector.z, vector.w);
}

