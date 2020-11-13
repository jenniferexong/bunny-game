#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "GuiRenderer.h"
#include "SkyboxRenderer.h"
#include "../scene/Scene.h"

class MasterRenderer {
private:
	std::shared_ptr<EntityShader> entity_shader_;
	std::shared_ptr<TerrainShader> terrain_shader_;
	EntityRenderer entity_renderer_;
	TerrainRenderer terrain_renderer_;
	GuiRenderer gui_renderer_;
	SkyboxRenderer skybox_renderer_;

	std::vector<Terrain> terrains_;
	std::vector<std::shared_ptr<GuiTexture>> guis_;

public:
	static glm::mat4 projection_matrix;

	MasterRenderer();
	void prepare(glm::mat4 proj_matrix);
	void renderScene(const shared_ptr<Scene>& scene);
	void render(const shared_ptr<Scene>& scene);
	void processTerrain(const Terrain& terrain);
	void processGui(const std::shared_ptr<GuiTexture>& gui);

	static void enableCulling();
	static void disableCulling();
};

