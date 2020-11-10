#pragma once

#include <memory>
#include <map>
#include <vector>

#include "../shaders/EntityShader.h"
#include "../objects/Entity.h"
#include "../models/Model.h"

struct CompareTexturedModel {
	bool operator()(const TexturedModel& a, const TexturedModel& b) const {
		return a.getTexture().getTextureId() < b.getTexture().getTextureId();
	}
};

class EntityRenderer {
private:
	std::shared_ptr<EntityShader> shader_;

	void prepareTexturedModel(const TexturedModel& model);
	void unbindTexturedModel();
	void loadTransformation(const Entity& entity);
	void loadTransformations(const std::vector<Entity>& entities, std::shared_ptr<std::vector<float>> float_data);

public:
	EntityRenderer(): shader_(nullptr) {}
	EntityRenderer(std::shared_ptr<EntityShader> shader) : shader_(std::move(shader)) {}

	void render(const std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities);

	void renderInstanced(const std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities);
};


