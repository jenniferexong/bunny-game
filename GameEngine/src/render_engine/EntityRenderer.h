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
	std::shared_ptr<DefaultShader> shader_;

	void prepareTexturedModel(const TexturedModel& model);
	void unbindTexturedModel();
	void loadTransformation(const Entity& entity);

public:
	EntityRenderer(): shader_(nullptr) {}
	EntityRenderer(std::shared_ptr<DefaultShader> shader) : shader_(std::move(shader)) {}
	void render(std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities);
};


