#include "Helper.h"

#include <glm/gtc/random.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

#include "Application.h"
#include "../../engine/util/FilePath.h"
#include "../../engine/environment/Environment.h"
#include "../game-scene/Player.h"

using namespace std;
using namespace glm;
namespace fs = std::filesystem;

void Helper::spawnEntity(
	shared_ptr<Player> player, 
	Environment& environment,
	TexturedModel& model,
	const fs::path& file,
	vec3 rotation,
	float scale)
{
	// write position to file
	ofstream positions(FilePath::get(file, FileType::Data), ios::app);
	vec3 position = player->getPosition();
	auto entity = make_shared<Entity>(model);

	positions << position.x << " " << position.z << endl;
	positions.close();

	Terrain terrain = environment.getTerrain(*player);
	float y = terrain.getHeightOfTerrain(position.x, position.z);
	vec3 terrain_normal = terrain.getNormalOfTerrain(position.x, position.z);
	entity->setPosition(position.x, y, position.z);
	entity->setRotation(linearRand(0.f, 90.f), 0, 0);
	entity->rotate(rotation.x, rotation.y, rotation.z);
	entity->setAlignmentRotation(terrain_normal);
	entity->setScale(scale);
	environment.addEntity(entity);
}

void Helper::loadPositionsFromFile(
	const Terrain& terrain,
	Environment& environment,
	TexturedModel& model,
	const fs::path& positions_file,
	vec3 rotation,
	float scale)
{
	float x, y, z;
	string line;

	string file_path = FilePath::get(positions_file, FileType::Data);
	ifstream file(file_path);
	if (!file.is_open()) Error::file(FileType::Data, file_path);

	while (getline(file, line)) {
		auto entity = make_shared<Entity>(model);

		stringstream str_stream(line);
		str_stream >> x >> z;

		y = terrain.getHeightOfTerrain(x, z);
		vec3 terrain_normal = terrain.getNormalOfTerrain(x, z);
		entity->setPosition(x, y, z);
		entity->setRotation(linearRand(0.f, 90.f), 0, 0);
		entity->rotate(rotation.x, rotation.y, rotation.z);
		entity->setAlignmentRotation(terrain_normal);
		entity->setScale(scale);
		environment.addEntity(entity);
	}
	file.close();
}

void Helper::loadPositionsFromFileToSet(
	const Terrain& terrain,
	set<shared_ptr<Entity>>& set,
	TexturedModel& model,
	const fs::path& positions_file,
	vec3 rotation,
	float scale)
{
	float x, y, z;
	string line;

	ifstream file(FilePath::get(positions_file, FileType::Data));
	while (getline(file, line)) {
		auto entity = make_shared<Entity>(model);

		stringstream str_stream(line);
		str_stream >> x >> z;

		y = terrain.getHeightOfTerrain(x, z);
		vec3 terrain_normal = terrain.getNormalOfTerrain(x, z);
		entity->setPosition(x, y, z);
		entity->setRotation(linearRand(0.f, 90.f), 0, 0);
		entity->rotate(rotation.x, rotation.y, rotation.z);
		entity->setAlignmentRotation(terrain_normal);
		entity->setScale(scale);
		set.insert(entity);
	}
	file.close();
}

shared_ptr<TerrainTexturePack> Helper::makeTexturePack(
	const fs::path& base, 
	const fs::path& red,
	const fs::path &green,
	const fs::path& blue) 
{
	Texture base_texture = Texture(Engine::instance->loader->loadTexture(base));
	Texture red_texture = Texture(Engine::instance->loader->loadTexture(red));
	Texture green_texture = Texture(Engine::instance->loader->loadTexture(green));
	Texture blue_texture = Texture(Engine::instance->loader->loadTexture(blue));

	return make_shared<TerrainTexturePack> (
		base_texture, red_texture, green_texture, blue_texture
	);
}

TexturedModel Helper::makeModel(
	const fs::path& obj_file,
	const fs::path& texture_file,
	const Material& material)
{
	InstancedMesh mesh = Engine::instance->loader->loadToVaoInstanced(obj_file);
	ModelTexture texture(Engine::instance->loader->loadTexture(texture_file), material);
	return TexturedModel(mesh, texture);
}
