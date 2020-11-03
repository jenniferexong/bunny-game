
#pragma once

#include <glm/glm.hpp>

class Light {
private:
	vec3 m_position;
	vec3 m_color;

public:
	Light(vec3 position, vec3 color) : m_position(position), m_color(color) {}

	void setPosition(vec3 position) { m_position = position; }
	void setColor(vec3 color) { m_color = color; }

	vec3 getPosition() const { return m_position; }
	vec3 getColor() const { return m_color; }
};