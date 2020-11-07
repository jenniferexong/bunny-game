#pragma once

#include <glm/glm.hpp>

#include "Entity.h"
#include "Terrain.h"

using glm::vec3;

class Player final: public Entity {
private:
	enum class DirectionState {
		N, E, S, W, NE, SE, SW, NW
	};

	static const float run_speed;
	static const float turn_speed;
	static const float gravity;
	static const float jump_power;

	DirectionState state_ = DirectionState::N;
	float forward_speed_ = 0;
	float side_speed_ = 0;
	float current_turn_speed_ = 0;
	float up_velocity_ = 0;

	bool is_in_air = false;

	void jump();
	void fall();
	glm::vec3 getRotationOffset();

public:
	Player() = default;
	Player(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale)
		: Entity(std::move(model), position, rotation, scale) {}

	void changeDirection(double amount);
	void updatePosition(const Terrain& terrain);
	void updateSpeed();
};
