#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "GuiRenderer.h"
#include "SkyboxRenderer.h"
#include "WaterFrameBuffers.h"
#include "WaterRenderer.h"

class Scene;
class GameScene;

class MasterRenderer {
private:
	std::shared_ptr<EntityShader> entity_shader_;
	std::shared_ptr<TerrainShader> terrain_shader_;

	EntityRenderer entity_renderer_;
	TerrainRenderer terrain_renderer_;
	GuiRenderer gui_renderer_;
	SkyboxRenderer skybox_renderer_;

	WaterFrameBuffers water_fbos_;
	WaterRenderer water_renderer_ = WaterRenderer(water_fbos_);

public:
	static glm::mat4 projection_matrix;
	static float fov;
	static float near_plane;
	static float far_plane;

	// update every frame
	static int window_width;
	static int window_height;

	MasterRenderer();
	void prepare(glm::mat4 proj_matrix);

	void renderEntities(const Environment& environment, glm::vec4 clipping_plane, bool progress_time);
	void renderTerrain(const Environment& environment, glm::vec4 clipping_plane, bool progress_time);
	void renderSkybox(const Environment& environment, bool progress_time);

	void renderWater(const Environment& environment);
	void renderWaterReflection(GameScene& scene, void(GameScene::*render_scene)(glm::vec4, bool));
	void renderWaterRefraction(GameScene& scene, void(GameScene::*render_scene)(glm::vec4, bool));

	void renderGui(const vector<shared_ptr<GuiTexture>>& guis);

	static void enableCulling();
	static void disableCulling();
};

