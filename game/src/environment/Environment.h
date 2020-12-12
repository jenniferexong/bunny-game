#pragma once

#include <unordered_map>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "../engine/models/Model.h"
#include "../environment/Entity.h"
#include "../environment/Light.h"
#include "../environment/Skybox.h"
#include "../environment/Terrain.h"
#include "../environment/Water.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::weak_ptr;
using std::unordered_set;
using std::vector;

class Camera;
class Player;

using entity_set = unordered_set<weak_ptr<Entity>>;
using entity_map = unordered_map<TexturedModel, entity_set>;

namespace std {
	template <>
	struct hash<TexturedModel>
	{
		size_t operator()(const TexturedModel& model) const
		{
			return hash<int>()(model.getTexture().getTextureId());
		}
	};
	template <>
	struct hash<weak_ptr<Entity>>
	{
		size_t operator()(const weak_ptr<Entity>& entity) const
		{
			return hash<int>()(entity.lock()->getEntityId());
		}
	};
	template <>
	struct equal_to<weak_ptr<Entity>>
	{
		bool operator() (const weak_ptr<Entity>& a, const weak_ptr<Entity>& b) const
		{
			return a.lock() == b.lock();
		}
	};
	
}

class Environment {
private:
	vector<shared_ptr<Entity>> entities_;
	entity_map entities_in_view_;

	vector<Terrain> terrains_;
	vector<Water> water_;
	vector<weak_ptr<Light>> lights_;
	weak_ptr<Light> sun_;
	weak_ptr<Skybox> skybox_;

	weak_ptr<Camera> camera_;

public:
	void addTerrain(const Terrain& terrain);
	void addEntity(const shared_ptr<Entity>& entity);
	void addEntitySet(const std::set<shared_ptr<Entity>>& entity);
	void addEntityToMap(entity_map& map, shared_ptr<Entity> entity);
	void addWater(const Water& water);
	void setLights(const vector<weak_ptr<Light>>& lights);
	void setCamera(const shared_ptr<Camera>& camera);
	void setSun(const shared_ptr<Light>& sun);
	void setSkybox(const shared_ptr<Skybox>& skybox);

	void updateInView();

	const vector<shared_ptr<Entity>>& getEntities() const { return entities_; }
	const vector<Terrain>& getTerrains() const { return terrains_; }
	const vector<Water>& getWater() const { return water_; }
	const vector<weak_ptr<Light>>& getLights() const { return lights_; }
	const weak_ptr<Light>& getSun() const { return sun_; }
	const weak_ptr<Camera>& getCamera() const { return camera_; }
	const weak_ptr<Skybox>& getSkybox() const { return skybox_; }
	const Terrain& getTerrain(shared_ptr<Player> player);

	const entity_map& getEntitiesInView() const { return entities_in_view_; }
};
