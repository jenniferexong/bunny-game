#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../models/Model.h"

class Shader;

using glm::vec3;

class Entity {
protected:
	std::shared_ptr<TexturedModel> _model; // pointer for run time polymorphism
	vec3 _position, _rotation; // yaw, pitch, roll
	float _scale;

public:
	Entity() : _model(nullptr), _position(vec3(0)), _rotation(vec3(0)), _scale(1) {}

	Entity(std::shared_ptr<TexturedModel> model) : _model(std::move(model)), _position(vec3(0)), _rotation(vec3(0)), _scale(1) {}

	Entity(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale) 
		: _model(std::move(model)), _position(position), _rotation(rotation), _scale(scale) {}

	void move(float dx, float dy, float dz);
	void rotate(float yaw, float pitch, float roll);

	// Setters
	void setPosition(float x, float y, float z) { _position = vec3(x, y, z); }
	void setRotation(float yaw, float pitch, float roll) { _rotation = vec3(yaw, pitch, roll); }
	void setScale(float scale) { _scale = scale; }

	// Getters
	TexturedModel getModel() const { return *_model; }
	vec3 getPosition() const { return _position; }
	vec3 getRotation() const { return _rotation; }
	float getScale() const { return _scale; }
};
