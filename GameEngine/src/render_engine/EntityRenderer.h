#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <map>
#include <vector>

#include "../shaders/DefaultShader.h"
#include "../objects/Entity.h"
#include "../objects/Light.h"
#include "../models/Model.h"

struct CompareTexturedModel {
	bool operator()(const TexturedModel& a, const TexturedModel& b) const {
		return a.getTexture().getTextureId() < b.getTexture().getTextureId();
	}
};

class EntityRenderer {
private:
	static const float s_fov;
	static const float s_near_plane;
	static const float s_far_plane;

	std::shared_ptr<DefaultShader> m_shader;

	void prepareTexturedModel(const TexturedModel& model);
	void unbindTexturedModel();
	void loadTransformation(const Entity& entity);

public:
	EntityRenderer(): m_shader(nullptr) {}
	EntityRenderer(std::shared_ptr<DefaultShader> shader) : m_shader(shader) {}
	void render(std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities);
};


