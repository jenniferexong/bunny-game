#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "GuiRenderer.h"
#include "SkyboxRenderer.h"
#include "WaterRenderer.h"

#include "../scene/Scene.h"

class MasterRenderer {
private:
	std::shared_ptr<EntityShader> entity_shader_;
	std::shared_ptr<TerrainShader> terrain_shader_;

	EntityRenderer entity_renderer_;
	TerrainRenderer terrain_renderer_;
	WaterRenderer water_renderer_;
	GuiRenderer gui_renderer_;
	SkyboxRenderer skybox_renderer_;

public:
	static glm::mat4 projection_matrix;

	MasterRenderer();
	void prepare(glm::mat4 proj_matrix);

	void renderAll(const shared_ptr<Scene>& scene);
	void renderScene(const shared_ptr<Scene>& scene);
	void render(const shared_ptr<Scene>& scene);

	static void enableCulling();
	static void disableCulling();
};
