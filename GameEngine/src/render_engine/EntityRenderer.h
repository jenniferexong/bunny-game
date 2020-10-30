#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <map>
#include <vector>

#include "../shaders/DefaultShader.h"
#include "../object/Entity.h"
#include "../object/Light.h"
#include "../models/Model.h"

struct CompareTexturedModel {
	bool operator()(const TexturedModel& a, const TexturedModel& b) const {
		return a.getTexture().getId() < b.getTexture().getId();
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
	void prepareInstance(const Entity& entity);

public:
	EntityRenderer(): m_shader(nullptr) {}
	EntityRenderer(std::shared_ptr<DefaultShader> shader) : m_shader(shader) {}
	void render(std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities);
};


