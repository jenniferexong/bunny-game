#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../objects/Player.h"

using glm::vec3;

class Camera {
private:
	static const float pitch;

	static float distance_from_player; // changed with mouse scroll

	static const float min_distance;
	static const float max_distance;

	std::shared_ptr<Player> player_;

	vec3 position_ = vec3(0);
	vec3 rotation_ = vec3(0, pitch, 0); // yaw, pitch, roll

public:
	Camera() = default;

	Camera(std::shared_ptr<Player> player): player_(std::move(player)) {}

	void updateView();
	void zoom(float amount);
	void print();

	vec3 getPosition() const { return position_; }
	vec3 getRotation() const { return rotation_; }
};

