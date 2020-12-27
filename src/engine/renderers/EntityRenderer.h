#pragma once

#include <memory>
#include <map>
#include <vector>
#include <unordered_set>
#include <string>

class Environment;
class EntityShader;
class Entity;
class TexturedModel;

using std::unordered_set;
using std::shared_ptr;
using std::weak_ptr;

class EntityRenderer {
private:
	static constexpr char name_[] = "EntityRenderer";

	std::shared_ptr<EntityShader> shader_;

	void prepareTexturedModel(const TexturedModel& model);
	void unbindTexturedModel();
	void loadTransformation(const Entity& entity);
	void loadTransformations(
		const unordered_set<weak_ptr<Entity>>& entities,
		std::vector<float>& float_data
	);
	void loadColors(
		const unordered_set<weak_ptr<Entity>>& entities,
		std::vector<float>& float_data
	);

public:
	EntityRenderer(std::shared_ptr<EntityShader> shader);

	EntityRenderer(): shader_(nullptr) {}
	~EntityRenderer() = default;

	void render(const Environment& environment);
	void renderInstanced(const Environment& environment);
};


