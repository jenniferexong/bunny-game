#pragma once

#include <unordered_map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../models/Model.h"
#include "../environment/Entity.h"
#include "../environment/Light.h"
#include "../environment/Skybox.h"
#include "../environment/Terrain.h"
#include "../environment/Water.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::set;
using std::vector;

class Camera;
class Player;

using entity_set = shared_ptr<set<shared_ptr<Entity>>>;
using entity_map = unordered_map<shared_ptr<TexturedModel>, entity_set>;

namespace std {
	template <>
	struct hash<shared_ptr<TexturedModel>>
	{
		size_t operator()(const shared_ptr<TexturedModel>& model) const
		{
			return hash<int>()(model->getTexture().getTextureId());
		}
	};
}

class Environment {
private:
	vector<shared_ptr<Entity>> entities_;
	entity_map entities_in_view_;

	vector<Terrain> terrains_;
	vector<Water> water_;
	vector<shared_ptr<Light>> lights_;
	shared_ptr<Light> sun_;
	Skybox skybox_;

	shared_ptr<Camera> camera_;

public:
	void addTerrain(const Terrain& terrain);
	void addEntity(const shared_ptr<Entity>& entity);
	void addEntityToMap(entity_map& map, shared_ptr<Entity> entity);
	void addWater(const Water& water);
	void addEntitySet(const entity_set& set);
	void setLights(const vector<shared_ptr<Light>>& lights);
	void setCamera(const shared_ptr<Camera>& camera);
	void setSun(const shared_ptr<Light>& sun);
	void setSkybox(const Skybox& skybox);

	void updateInView();

	const vector<shared_ptr<Entity>> getEntities() const { return entities_; }
	const vector<Terrain>& getTerrains() const { return terrains_; }
	const vector<Water>& getWater() const { return water_; }
	const vector<shared_ptr<Light>>& getLights() const { return lights_; }
	const shared_ptr<Light>& getSun() const { return sun_; }
	const shared_ptr<Camera>& getCamera() const { return camera_; }
	const Skybox& getSkybox() const { return skybox_; }
	const Terrain& getTerrain(shared_ptr<Player> player);

	const entity_map& getEntitiesInView() const { return entities_in_view_; }
};
