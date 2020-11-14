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
#include "../environment/Terrain.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::set;
using std::vector;

class Camera;

class Environment {
private:
	unordered_map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, TexturedModelHash, TexturedModelEquality> entities_;
	vector<Terrain> terrains_;
	vector<shared_ptr<Light>> lights_;
	shared_ptr<Light> sun_;
	shared_ptr<Camera> camera_;
	//skybox class
	// Skybox skybox;

public:
	void addTerrain(const Terrain& terrain);
	void addEntity(const shared_ptr<Entity>& entity);
	void addEntitySet(const shared_ptr<set<shared_ptr<Entity>>>& entity_set);
	void setLights(const vector<shared_ptr<Light>>& lights);
	void setCamera(const shared_ptr<Camera>& camera);
	void setSun(const shared_ptr<Light>& sun);
	void setSkybox();

	const unordered_map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, TexturedModelHash, TexturedModelEquality>& getEntities() const { return entities_; }
	const vector<Terrain>& getTerrains() const { return terrains_; }
	const vector<shared_ptr<Light>>& getLights() const { return lights_; }
	const shared_ptr<Light>& getSun() const { return sun_; }
	const shared_ptr<Camera>& getCamera() const { return camera_; }
	//virtual const Skybox& getSkybox() { return lights_; } //how to check if there isn't one
};
