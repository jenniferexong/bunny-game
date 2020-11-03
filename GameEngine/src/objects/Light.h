
#pragma once

#include <glm/glm.hpp>

class Light {
private:
	vec3 _position;
	vec3 _color;

public:
	Light(vec3 position, vec3 color) : _position(position), _color(color) {}

	void setPosition(vec3 position) { _position = position; }
	void setColor(vec3 color) { _color = color; }

	vec3 getPosition() const { return _position; }
	vec3 getColor() const { return _color; }
};