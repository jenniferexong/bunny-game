#include "Environment.h"

const Terrain& Environment::getTerrain(shared_ptr<Player> player)
{
	return terrains_.at(0);
}


void Environment::addEntity(const shared_ptr<Entity>& entity)
{
	const auto& model = entity->getModel();
	if (entities_.find(model) == entities_.end()) {
		auto entity_set = std::make_shared<set<shared_ptr<Entity>>>();
		entities_.insert({ model, entity_set });
	}
	auto entity_set = entities_.at(model);
	entity_set->insert(entity);
}

void Environment::addEntitySet(const entity_set& set)
{
	for (const auto& e: *set)
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

void Environment::setLights(const vector<shared_ptr<Light>>& lights)
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


void Environment::setSkybox(const Skybox& skybox)
{
	skybox_ = skybox;
}



