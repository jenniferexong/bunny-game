#pragma once

#include <glm/glm.hpp>
#include <string>

using message = const std::string&;

/**
 * Logging
 */
namespace Log {
	const bool print = true;

	// no indent
	void init(message object, bool success);
	void destroy(message object, int id);

	// 1 indent
	void texture(message texture_name, int id);
	void fbo(message attachment, int id);
	void s(message message);
	void s(int num);

	//
	void matrix(message message, glm::mat4 matrix);
	void vector(message message, glm::vec3 vector);
	void vector(message message, glm::vec4 vector);
	void vector(glm::vec2 vector);
	void vector(glm::vec3 vector);
	void vector(glm::vec4 vector);
	void val(message message, float value);
	void val(message message, int value);
}

/**
 * Prints a message before exiting program
 */
namespace Error {
	static constexpr char line[] = "#########################################";

	void file(const std::string& type, const std::string& path);

	void gl_check(const std::string& location);

	void exit(const std::string& message);
}
