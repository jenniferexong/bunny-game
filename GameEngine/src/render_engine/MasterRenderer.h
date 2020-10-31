#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"

class MasterRenderer {
private:
	std::shared_ptr<DefaultShader> m_entity_shader;
	std::shared_ptr<TerrainShader> m_terrain_shader;
	EntityRenderer m_entity_renderer;
	TerrainRenderer m_terrain_renderer;

	std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel> m_entities;
	std::vector<Terrain> m_terrains;

public:
	static glm::mat4 s_projection_matrix;
	static const float s_fov;
	static const float s_near_plane;
	static const float s_far_plane;

	MasterRenderer();
	void prepare();
	void render(const Light& sun);
	void processEntity(const Entity& entity);
	void processTerrain(const Terrain& terrain);

	static void enableCulling();
	static void disableCulling();
};

