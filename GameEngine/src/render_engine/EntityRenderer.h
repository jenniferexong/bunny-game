#pragma once

#include <memory>
#include <map>
#include <vector>
#include <set>

#include "../shaders/EntityShader.h"
#include "../objects/Entity.h"
#include "../models/Model.h"
#include "../CompareTexturedModel.h"

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

public:
	EntityRenderer(): shader_(nullptr) {}
	EntityRenderer(std::shared_ptr<EntityShader> shader) : shader_(std::move(shader)) {}

	void render(const map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, CompareTexturedModel>& entities);

	void renderInstanced(const map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, CompareTexturedModel>& entities);
};


