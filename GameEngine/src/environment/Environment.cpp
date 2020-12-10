#include "Environment.h"

#include "../scene/Camera.h"

const Terrain& Environment::getTerrain(shared_ptr<Player> player)
{
	return terrains_.at(0);
}

void Environment::addEntityToMap(entity_map& map, shared_ptr<Entity> entity)
{
	const auto& model = entity->getModel();
	if (map.find(model) == map.end()) {
		unordered_set<weak_ptr<Entity>> entity_set;
		map.insert({ model, entity_set });
	}
	auto& entity_set = map.at(model);
	entity_set.insert(entity);
}

void Environment::addEntity(const shared_ptr<Entity>& entity)
{
	entities_.push_back(entity);
}

void Environment::addEntitySet(const std::set<shared_ptr<Entity>>& entities)
{
	for (const auto& e : entities)
		addEntity(e);
}

void Environment::addTerrain(const Terrain& terrain)
{
	terrains_.push_back(terrain);
}

void Environment::addWater(const Water& water)
{
	water_.push_back(water);
}

void Environment::setLights(const vector<weak_ptr<Light>>& lights)
{
	lights_ = lights;
}

void Environment::setSun(const shared_ptr<Light>& sun)
{
	sun_ = sun;
}

 void Environment::setCamera(const shared_ptr<Camera>& camera)
 {
	 camera_ = camera;
 }


void Environment::setSkybox(const shared_ptr<Skybox>& skybox)
{
	skybox_ = skybox;
}

void Environment::updateInView()
{
	int count = 0;
	entities_in_view_.clear();
	for (const auto& entity: entities_) {
		if (camera_.lock()->canSeePoint(entity->getPosition())) {
			addEntityToMap(entities_in_view_, entity);
			count++;
		}
	}
	//Print::val("Entities in view", count);
}




