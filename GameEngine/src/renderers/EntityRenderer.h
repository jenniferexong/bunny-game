#pragma once

#include <memory>
#include <map>
#include <vector>
#include <set>

#include "../shaders/EntityShader.h"
#include "../environment/Entity.h"
#include "../models/Model.h"

using std::map;
using std::set;
using std::shared_ptr;

class EntityRenderer {
private:
	std::shared_ptr<EntityShader> shader_;

	void prepareTexturedModel(const TexturedModel& model);
	void unbindTexturedModel();
	void loadTransformation(const Entity& entity);
	void loadTransformations(const set<shared_ptr<Entity>>& entities, shared_ptr<std::vector<float>> float_data);
	void loadColors(const std::set<shared_ptr<Entity>>& entities, std::shared_ptr<std::vector<float>> float_data);

public:
	EntityRenderer(): shader_(nullptr) {}
	EntityRenderer(std::shared_ptr<EntityShader> shader) : shader_(std::move(shader)) {}

	void render(const Environment& environment);

	void renderInstanced(const Environment& environment);
};


