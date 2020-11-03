#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

using glm::vec3;

class Player final: public Entity {
private:
	static const float run_speed;
	static const float turn_speed;

	float _current_speed = 0;
	float _current_turn_speed = 0;

public:
	Player() = default;
	Player(std::shared_ptr<TexturedModel> model, vec3 position, vec3 rotation, float scale)
		: Entity(std::move(model), position, rotation, scale) {}

	void updatePosition();
	void updateSpeed();
};
