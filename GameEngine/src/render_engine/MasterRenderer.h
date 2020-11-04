#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"

class MasterRenderer {
private:
	std::shared_ptr<DefaultShader> entity_shader_;
	std::shared_ptr<TerrainShader> terrain_shader_;
	EntityRenderer entity_renderer_;
	TerrainRenderer terrain_renderer_;

	std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel> entities_;
	std::vector<Terrain> terrains_;

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

