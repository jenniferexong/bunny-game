#pragma once

#include <memory>
#include <map>
#include <vector>
#include <unordered_set>

class Environment;
class EntityShader;
class Entity;
class TexturedModel;

using std::map;
using std::unordered_set;
using std::shared_ptr;
using std::weak_ptr;

class EntityRenderer {
private:
	std::shared_ptr<EntityShader> shader_;

	void prepareTexturedModel(const TexturedModel& model);
	void unbindTexturedModel();
	void loadTransformation(const Entity& entity);
	void loadTransformations(const unordered_set<weak_ptr<Entity>>& entities, std::vector<float>& float_data);
	void loadColors(const unordered_set<weak_ptr<Entity>>& entities, std::vector<float>& float_data);

public:
	EntityRenderer(): shader_(nullptr) {}
	EntityRenderer(std::shared_ptr<EntityShader> shader) : shader_(std::move(shader)) {}
	~EntityRenderer() = default;

	void render(const Environment& environment);
	void renderInstanced(const Environment& environment);
};


