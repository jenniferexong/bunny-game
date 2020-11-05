#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

using glm::vec3;

class Player final: public Entity {
private:
	static const float run_speed;
	static const float turn_speed;
	static const float gravity;
	static const float jump_power;
	static const float terrain_height;

	float forward_speed_ = 0;
	float side_speed_ = 0;
	float current_turn_speed_ = 0;
	float up_velocity_ = 0;

	void jump();
	void fall();

public:
	Player() = default;
	Player(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale)
		: Entity(std::move(model), position, rotation, scale) {}

	void changeDirection(double amount);
	void updatePosition();
	void updateSpeed();
};
