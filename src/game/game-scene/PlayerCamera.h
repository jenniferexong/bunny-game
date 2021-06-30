#include "../../engine/Camera.h"
#include "../../engine/util/Log.h"

class Player;
class Water;
class Terrain;

#include <memory>
#include <vector>

class PlayerCamera: public Camera {
private:
	static float distance_from_player; // changed with mouse scroll

	static const float min_distance;
	static const float max_distance;

	std::shared_ptr<Player> player_;

public:
	static const float turn_speed;

	PlayerCamera() { Log::init("PlayerView", true); }

	PlayerCamera(std::shared_ptr<Player> player): player_(std::move(player)) {}

	void updateView(const Terrain& terrain, const std::vector<Water>& water);
	void zoom(float amount, const std::vector<Water>& water);
	void changePitch(float amount);

	void positionForReflection(float water_height);
	void positionForRefraction(float water_height);
};
