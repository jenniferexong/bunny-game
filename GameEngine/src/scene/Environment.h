#pragma once

#include <unordered_map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../CompareTexturedModel.h"
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

using entity_set = shared_ptr<set<shared_ptr<Entity>>>;
using entity_map = unordered_map<shared_ptr<TexturedModel>, entity_set, TexturedModelHash, TexturedModelEquality>; //TODO

class Environment {
private:
	entity_map entities_;

	vector<Terrain> terrains_;
	vector<Water> water_;
	vector<shared_ptr<Light>> lights_;
	shared_ptr<Light> sun_;
	shared_ptr<Camera> camera_;
	Skybox skybox_;

public:
	void addTerrain(const Terrain& terrain);
	void addEntity(const shared_ptr<Entity>& entity);
	void addWater(const Water& water);
	void addEntitySet(const entity_set& set);
	void setLights(const vector<shared_ptr<Light>>& lights);
	void setCamera(const shared_ptr<Camera>& camera);
	void setSun(const shared_ptr<Light>& sun);
	void setSkybox(const Skybox& skybox);

	const entity_map& getEntities() const { return entities_; }
	const vector<Terrain>& getTerrains() const { return terrains_; }
	const vector<Water>& getWater() const { return water_; }
	const vector<shared_ptr<Light>>& getLights() const { return lights_; }
	const shared_ptr<Light>& getSun() const { return sun_; }
	const shared_ptr<Camera>& getCamera() const { return camera_; }
	const Skybox& getSkybox() const { return skybox_; }
};
