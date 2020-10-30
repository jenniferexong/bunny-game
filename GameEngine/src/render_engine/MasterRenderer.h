#pragma once

#include "EntityRenderer.h"

class MasterRenderer {
private:
	std::shared_ptr<DefaultShader> m_shader;
	EntityRenderer m_renderer;

	std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel> m_entities;

public:
	static glm::mat4 s_projection_matrix;
	static const float s_fov;
	static const float s_near_plane;
	static const float s_far_plane;

	MasterRenderer() : m_shader(nullptr) {}
	MasterRenderer(const DefaultShader& shader);
	void prepare();
	void render(const Light& sun);
	void processEntity(const Entity& entity);
};

