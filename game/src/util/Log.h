#pragma once

#include <glm/glm.hpp>

#include <string>

using message = const std::string&;

namespace Print {
	const bool print = true;
	void matrix(message message, glm::mat4 matrix);
	void vector(message message, glm::vec3 vector);
	void vector(message message, glm::vec4 vector);
	void vector(glm::vec3 vector);
	void vector(glm::vec4 vector);
	void val(message message, float value);
	void val(message message, int value);
	void texture(message texture_name, int id);
	void fbo(message attachment, int id);

	void s(message message);
	void s(int num);

	void init(message object, bool success);
}
