#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Print {
	const bool print = true;
	void matrix(std::string title, glm::mat4 matrix);
	void vector(std::string title, glm::vec3 vector);
	void vector(std::string title, glm::vec4 vector);
	void vector(glm::vec3 vector);
	void vector(glm::vec4 vector);
	void val(std::string title, float value);
	void val(std::string title, int value);
	void texture(std::string texture_name, int id);
	void fbo(std::string attachment, int id);
	void s(const std::string& str);
}
