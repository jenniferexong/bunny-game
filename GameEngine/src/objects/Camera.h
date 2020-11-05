#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "../objects/Player.h"

using glm::vec3;

class Camera {
private:
	static const float height_above_player;
	static const float distance_behind_player;

	std::shared_ptr<Player> player_;

	vec3 position_ = vec3(0);
	vec3 rotation_ = vec3(0); // yaw, pitch, roll

public:
	Camera() = default;

	Camera(std::shared_ptr<Player> player): player_(std::move(player)) {}

	void updateView();
	void print();

	vec3 getPosition() const { return position_; }
	vec3 getRotation() const { return rotation_; }
};

