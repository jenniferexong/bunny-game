#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../models/Model.h"

class Shader;

using glm::vec3;

class Entity {
protected:
	std::shared_ptr<TexturedModel> model_; // pointer for run time polymorphism
	vec3 position_, rotation_; // yaw, pitch, roll
	vec3 default_rotation_offset_ = vec3(0);
	vec3 rotation_offset_;
	glm::mat4 alignment_rotation_ = glm::mat4(1);
	float scale_;

	bool selectable_ = true;
	float brightness_ = 0.8f;

public:

	Entity() : model_(nullptr), position_(vec3(0)), rotation_(vec3(0)), scale_(1) {}

	Entity(std::shared_ptr<TexturedModel> model) : model_(std::move(model)), position_(vec3(0)), rotation_(vec3(0)), scale_(1) {}

	Entity(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale) 
		: model_(std::move(model)), position_(position), rotation_(rotation), scale_(scale) {}

	void move(float dx, float dy, float dz);
	void rotate(float yaw, float pitch, float roll);

	// Setters
	void setPosition(float x, float y, float z) { position_ = vec3(x, y, z); }
	void setRotation(float yaw, float pitch, float roll) { rotation_ = vec3(yaw, pitch, roll); }
	void setRotationOffset(float yaw, float pitch, float roll);
	void setAlignmentRotation(vec3 surface_normal);
	void setScale(float scale) { scale_ = scale; }

	// Getters
	std::shared_ptr<TexturedModel> getModel() const { return model_; }
	vec3 getPosition() const { return position_; }
	vec3 getRotation() const { return rotation_; }
	vec3 getActualRotation() const { return rotation_ + rotation_offset_; }
	glm::mat4 getAlignmentRotation() const { return alignment_rotation_; }
	float getScale() const { return scale_; }
	bool isSelectable() const { return selectable_; }
	float getBrightness() const { return brightness_; }

	void highlight();
	void unhighlight();
};