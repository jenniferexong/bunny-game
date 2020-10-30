#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <map>
#include <vector>

#include "../shaders/DefaultShader.h"
#include "../Entity.h"
#include "../Light.h"
#include "Model.h"

struct CompareTexturedModel {
	bool operator()(const TexturedModel& a, const TexturedModel& b) const {
		return a.getTexture().getId() < b.getTexture().getId();
	}
};

class Renderer {
private:
	static const float s_fov;
	static const float s_near_plane;
	static const float s_far_plane;

	std::shared_ptr<DefaultShader> m_shader;

	void prepareTexturedModel(const TexturedModel& model);
	void unbindTexturedModel();
	void prepareInstance(const Entity& entity);

public:
	Renderer(): m_shader(nullptr) {}
	Renderer(std::shared_ptr<DefaultShader> shader) : m_shader(shader) {}
	static glm::mat4 s_projection_matrix;
	void prepare();
	void render(std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities);
	//void render(const Entity& entity, const Shader& shader);
};

class MasterRenderer {
private:
	std::shared_ptr<DefaultShader> m_shader;
	Renderer m_renderer;

	std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel> m_entities;

public:
	MasterRenderer() : m_shader(nullptr) {}
	MasterRenderer(const DefaultShader& shader);
	void render(const Light& sun);
	void processEntity(const Entity& entity);
};

