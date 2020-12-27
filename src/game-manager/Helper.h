#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <set>
#include <string>

class TexturedModel;
class TerrainTexturePack;
class Terrain;
class Environment;
class Player;
class Entity;
struct Material;

namespace Helper {
	TexturedModel makeModel(
		const std::string& obj_name,
		const std::string& texture_name,
		const Material& material);

	std::shared_ptr<TerrainTexturePack> makeTexturePack(
		const std::string& base,
		const std::string& red,
		const std::string& green,
		const std::string& blue);

	void loadPositionsFromFile(
		const Terrain& terrain,
		Environment& environment,
		TexturedModel& model, 
		const std::string& name,
		glm::vec3 rotation,
		float scale);

	void loadPositionsFromFileToSet(
		const Terrain& terrain,
		std::set<std::shared_ptr<Entity>>& set,
		TexturedModel& model, 
		const std::string& name,
		glm::vec3 rotation,
		float scale);

	void spawnEntity(
		std::shared_ptr<Player> player,
		Environment& environment,
		TexturedModel& model,
		const std::string& name,
		glm::vec3 rotation,
		float scale);
}
