#include "Log.h"

#include <glm/gtc/matrix_access.hpp>

#include <iostream>

using std::string;
using std::cout;
using std::endl;

void Print::init(message object, bool success) {
	if (!Print::print)
		return;
	string output = "init: ";
	output.append(object);
	if (success)
		output.append(", success!");
	cout << output << endl;
}

void Print::val(message message, float value)
{
	if (!Print::print)
		return;
	cout << "-------------------" << endl;
	cout << message << ": " << value << endl;
}

void Print::val(message message, int value)
{
	if (!Print::print)
		return;
	cout << "-------------------" << endl;
	cout << message << ": " << value << endl;
}

void Print::s(int num)
{
	if (!Print::print)
		return;

	cout << "[ " <<  num  << " ]" << endl;
}

void Print::s(message message)
{
	if (!Print::print)
		return;
	cout << '\t' << message << endl;
}

void Print::texture(message texture_name, int id)
{
	if (!Print::print)
		return;
	printf("\tloaded texture: %s, %d\n", texture_name.c_str(), id);
}

void Print::fbo(message attachment, int id)
{
	if (!Print::print)
		return;

	printf("\tcreated fbo (%s): %d\n", attachment.c_str(), id);
}

void Print::matrix(message message, glm::mat4 matrix)
{
	if (!Print::print)
		return;
	using glm::vec4;
	vec4 r1 = glm::row(matrix, 0);
	vec4 r2 = glm::row(matrix, 1);
	vec4 r3 = glm::row(matrix, 2);
	vec4 r4 = glm::row(matrix, 3);

	cout << "-------------------" << endl;
	cout << message << endl;
	vector(r1);
	vector(r2);
	vector(r3);
	vector(r4);
}

void Print::vector(message message, glm::vec4 vector)
{
	if (!Print::print)
		return;
	cout << "-------------------" << endl;
	cout << message << endl;
	Print::vector(vector);
}

void Print::vector(message message, glm::vec3 vector)
{
	if (!Print::print)
		return;
	cout << "-------------------" << endl;
	cout << message << endl;
	Print::vector(vector);
}

void Print::vector(glm::vec3 vector)
{
	if (!Print::print)
		return;
	printf("%.2f, %.2f, %.2f\n", vector.x, vector.y, vector.z);
}

void Print::vector(glm::vec4 vector)
{
	if (!Print::print)
		return;
	printf("%.2f, %.2f, %.2f, %.2f\n", vector.x, vector.y, vector.z, vector.w);
}

