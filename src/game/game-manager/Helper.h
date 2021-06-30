#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <set>
#include <string>
#include <filesystem>

class TexturedModel;
class TerrainTexturePack;
class Terrain;
class Environment;
class Player;
class Entity;
struct Material;

namespace Helper {
	TexturedModel makeModel(
		const std::filesystem::path& obj_file,
		const std::filesystem::path& texture_file,
		const Material& material);

	std::shared_ptr<TerrainTexturePack> makeTexturePack(
		const std::filesystem::path& base,
		const std::filesystem::path& red,
		const std::filesystem::path& green,
		const std::filesystem::path& blue);

	void loadPositionsFromFile(
		const Terrain& terrain,
		Environment& environment,
		TexturedModel& model, 
		const std::filesystem::path& file,
		glm::vec3 rotation,
		float scale);

	void loadPositionsFromFileToSet(
		const Terrain& terrain,
		std::set<std::shared_ptr<Entity>>& set,
		TexturedModel& model, 
		const std::filesystem::path& file,
		glm::vec3 rotation,
		float scale);

	void spawnEntity(
		std::shared_ptr<Player> player,
		Environment& environment,
		TexturedModel& model,
		const std::filesystem::path& file,
		glm::vec3 rotation,
		float scale);
}
