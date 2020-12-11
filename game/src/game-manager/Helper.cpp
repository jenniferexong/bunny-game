#include "Helper.h"

#include <glm/gtc/random.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

#include "Application.h"
#include "FilePath.h"
#include "../scene/Player.h"
#include "../pkg/Model.h"
#include "../pkg/Environment.h"
#include "../engine/Error.h"

using namespace std;
using namespace glm;

void Helper::spawnEntity(shared_ptr<Player> player, Environment& environment, TexturedModel& model, const std::string& name, vec3 rotation, float scale)
{
	// write position to file
	ofstream positions(FilePath::data_path + name + "-positions.txt", ios::app);
	vec3 position = player->getPosition();
	auto entity = make_shared<Entity>(model);

	positions << position.x << " " << position.z << endl;
	positions.close();

	Terrain terrain = environment.getTerrain(player);
	float y = terrain.getHeightOfTerrain(position.x, position.z);
	vec3 terrain_normal = terrain.getNormalOfTerrain(position.x, position.z);
	entity->setPosition(position.x, y, position.z);
	entity->setRotation(linearRand(0.f, 90.f), 0, 0);
	entity->rotate(rotation.x, rotation.y, rotation.z);
	entity->setAlignmentRotation(terrain_normal);
	entity->setScale(scale);
	environment.addEntity(entity);
}

void Helper::loadPositionsFromFile(const Terrain& terrain, Environment& environment, TexturedModel& model, const std::string& name, vec3 rotation, float scale)
{
	float x, y, z;
	string line;

	ifstream file(FilePath::data_path + name + "-positions.txt");
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

void Helper::loadPositionsFromFileToSet(const Terrain& terrain, set<shared_ptr<Entity>>& set, TexturedModel& model,
	const string& name, vec3 rotation, float scale)
{
	float x, y, z;
	string line;

	ifstream file(FilePath::data_path + name + "-positions.txt");
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

shared_ptr<TerrainTexturePack> Helper::makeTexturePack(const string& base, const string& red,
	const string &green, const string& blue) 
{
	Texture base_texture = Texture(engine->loader->loadTexture(base));
	Texture red_texture = Texture(engine->loader->loadTexture(red));
	Texture green_texture = Texture(engine->loader->loadTexture(green));
	Texture blue_texture = Texture(engine->loader->loadTexture(blue));

	return make_shared<TerrainTexturePack>(base_texture, red_texture, green_texture, blue_texture);
}

TexturedModel Helper::makeModel(const string& obj_name,
	const string& texture_name, const Material& material)
{
	InstancedMesh mesh = engine->loader->loadToVaoInstanced(obj_name);
	ModelTexture texture(engine->loader->loadTexture(texture_name), material);
	return TexturedModel(mesh, texture);
}
