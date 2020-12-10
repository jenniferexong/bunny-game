#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <map>

#include "../environment/Entity.h"
#include "../game-manager/UserInput.h"

class Water;
class Environment;
class TexturedModel;

using glm::vec3;

class Player final: public Entity {
private:
	enum class DirectionState {
		N, E, S, W, NE, SE, SW, NW
	};

	static constexpr float run_speed = 30.f;
	static constexpr float gravity = -70.f;
	static constexpr float jump_power = 30.f;

	DirectionState state_ = DirectionState::N;
	float forward_speed_ = 0;
	float side_speed_ = 0;
	float up_velocity_ = 0;

	bool is_in_air_ = false;

	void jump();
	void fall();
	glm::vec3 getRotationOffset();

public:
	Player() = default;
	~Player() = default;
	Player(TexturedModel model, vec3 position, vec3 rotation, float scale);

	bool isMoving() const { return (forward_speed_ != 0 || side_speed_ != 0); }
	bool isInAir() const { return is_in_air_; }
	bool isInWater(const std::vector<Water>& water) const;

	void changeDirection(float amount);
	void updatePosition(const Environment& environment, const std::map<Key, bool>& move_keys);
	void updateSpeed(const std::map<Key, bool>& move_keys);
};
