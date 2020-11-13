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
	static const float gravity;
	static const float jump_power;

	DirectionState state_ = DirectionState::N;
	float forward_speed_ = 0;
	float side_speed_ = 0;
	float up_velocity_ = 0;

	vec3 initial_direction_ = vec3(0, 0, -1.f);

	bool is_in_air_ = false;

	void jump();
	void fall();
	glm::vec3 getRotationOffset();

public:
	Player() = default;
	Player(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale);

	vec3 getInitialDirection() const { return initial_direction_; }
	bool isMoving() const { return (forward_speed_ != 0 || side_speed_ != 0); }
	bool isInAir() const { return is_in_air_; }

	void changeDirection(double amount);
	void updatePosition(const Terrain& terrain);
	void updateSpeed();
};
