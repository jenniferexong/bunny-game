#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"

class MasterRenderer {
private:
	std::shared_ptr<DefaultShader> _entity_shader;
	std::shared_ptr<TerrainShader> _terrain_shader;
	EntityRenderer _entity_renderer;
	TerrainRenderer _terrain_renderer;

	std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel> _entities;
	std::vector<Terrain> _terrains;

public:
	static glm::mat4 projection_matrix;
	static const float fov;
	static const float near_plane;
	static const float far_plane;

	MasterRenderer();
	void prepare();
	void render(const Light& sun);
	void processEntity(const Entity& entity);
	void processTerrain(const Terrain& terrain);

	static void enableCulling();
	static void disableCulling();
};

