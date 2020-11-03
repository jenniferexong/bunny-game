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

	float _current_speed = 0;
	float _current_turn_speed = 0;
	float _up_velocity = 0;

	void jump();
	void fall();

public:
	Player() = default;
	Player(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale)
		: Entity(std::move(model), position, rotation, scale) {}

	void updatePosition();
	void updateSpeed();
};
