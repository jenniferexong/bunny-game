#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "../engine/FrustumBox.h"

class Terrain;
class Player;
class Water;

using std::vector;
using glm::vec3;

class Camera {
private:
	static float distance_from_player; // changed with mouse scroll

	static const float min_distance;
	static const float max_distance;

	std::shared_ptr<Player> player_;
	FrustumBox view_frustum_box_;

	vec3 position_ = vec3(0);
	vec3 rotation_ = vec3(0, 0.1f, 0); // yaw, pitch, roll

public:
	static const float turn_speed;

	Camera() = default;
	~Camera() = default;

	Camera(std::shared_ptr<Player> player): player_(std::move(player)) {}

	void updateView(const Terrain& terrain, const std::vector<Water>& water);
	void zoom(float amount, const vector<Water>& water);
	void print();
	void changePitch(float amount);

	void positionForReflection(float water_height);
	void positionForRefraction(float water_height);

	vec3 getPosition() const { return position_; }
	vec3 getRotation() const { return rotation_; }
	bool canSeePoint(vec3 point);
};

