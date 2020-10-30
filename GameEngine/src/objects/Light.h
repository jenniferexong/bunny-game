
#pragma once

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 m_position;
	glm::vec3 m_color;

public:
	Light(glm::vec3 position, glm::vec3 color) : m_position(position), m_color(color) {}

	inline void setPosition(glm::vec3 position) { m_position = position; }
	inline void setColor(glm::vec3 color) { m_color = color; }

	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getColor() const { return m_color; }
};