#include "Helper.h"

#include <glm/gtc/matrix_access.hpp>

#include <iostream>

using std::cout;
using std::endl;

void Print::val(std::string title, float value)
{
	cout << "-------------------" << endl;
	cout << title << ": " << value << endl;
}

void Print::val(std::string title, int value)
{
	cout << "-------------------" << endl;
	cout << title << ": " << value << endl;
}

void Print::texture(std::string texture_name, int id)
{
	printf("Loaded texture: %s, %d\n", texture_name.c_str(), id);
}

void Print::matrix(std::string title, glm::mat4 matrix)
{
	using glm::vec4;
	vec4 r1 = glm::row(matrix, 0);
	vec4 r2 = glm::row(matrix, 1);
	vec4 r3 = glm::row(matrix, 2);
	vec4 r4 = glm::row(matrix, 3);

	cout << "-------------------" << endl;
	cout << title << endl;
	vector(r1);
	vector(r2);
	vector(r3);
	vector(r4);
}

void Print::vector(std::string title, glm::vec4 vector)
{
	cout << "-------------------" << endl;
	cout << title << endl;
	Print::vector(vector);
}

void Print::vector(std::string title, glm::vec3 vector)
{
	cout << "-------------------" << endl;
	cout << title << endl;
	Print::vector(vector);
}

void Print::vector(glm::vec3 vector)
{
	printf("%.2f, %.2f, %.2f\n", vector.x, vector.y, vector.z);
}

void Print::vector(glm::vec4 vector)
{
	printf("%.2f, %.2f, %.2f, %.2f\n", vector.x, vector.y, vector.z, vector.w);
}

