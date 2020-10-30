#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../models/Model.h"

class Shader;

using glm::vec3;

class Entity {
private:
	std::shared_ptr<TexturedModel> m_model; // pointer for run time polymorphism
	vec3 m_position, m_rotation; // yaw, pitch, roll
	float m_scale;

public:
	Entity() : m_model(nullptr), m_position(vec3(0)), m_rotation(vec3(0)), m_scale(1) {}

	Entity(std::shared_ptr<TexturedModel> model) : m_model(model), m_position(vec3(0)), m_rotation(vec3(0)), m_scale(1) {}

	Entity(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale) 
		: m_model(model), m_position(position), m_rotation(rotation), m_scale(scale) {}

	void move(float dx, float dy, float dz);
	void rotate(float yaw, float pitch, float roll);

	// Setters
	inline void setPosition(float x, float y, float z) { m_position = vec3(x, y, z); }
	inline void setRotation(float yaw, float pitch, float roll) { m_rotation = vec3(yaw, pitch, roll); }
	inline void setScale(float scale) { m_scale = scale; }

	// Getters
	inline const TexturedModel getModel() const { return *m_model; }
	inline const vec3 getPosition() const { return m_position; }
	inline const vec3 getRotation() const { return m_rotation; }
	inline const float getScale() const { return m_scale; }
};
