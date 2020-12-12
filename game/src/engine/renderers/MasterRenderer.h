#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "GuiRenderer.h"
#include "SkyboxRenderer.h"
#include "TextRenderer.h"
#include "WaterRenderer.h"
#include "../post-processing/PostProcessor.h"

class Scene;
class GameScene;

using std::shared_ptr;

class MasterRenderer {
private:
	shared_ptr<EntityShader> entity_shader_;
	shared_ptr<TerrainShader> terrain_shader_;

	EntityRenderer entity_renderer_;
	TerrainRenderer terrain_renderer_;
	SkyboxRenderer skybox_renderer_;
	WaterRenderer water_renderer_;

	TextRenderer text_renderer_;
	GuiRenderer gui_renderer_;

public:
	static glm::mat4 projection_matrix;
	static float fov;
	static float near_plane;
	static float far_plane;

	MasterRenderer();
	void resize(int width, int height);
	void prepare(glm::mat4 proj_matrix);
	~MasterRenderer() = default;

	void renderEntities(const Environment& environment, glm::vec4 clipping_plane);
	void renderTerrain(const Environment& environment, glm::vec4 clipping_plane);
	void renderSkybox(const Environment& environment);

	void renderWater(const Environment& environment);
	void renderWaterReflection(GameScene& scene, void(GameScene::*render_scene)(glm::vec4));
	void renderWaterRefraction(GameScene& scene, void(GameScene::*render_scene)(glm::vec4));

	void renderGui(const unordered_set<weak_ptr<GuiTexture>>& guis);
	void renderText(const TextMaster& text_master);


	static void enableCulling();
	static void disableCulling();
	static void enableAlphaBlending();
	static void disableAlphaBlending();
	static void enableDepthTest();
	static void disableDepthTest();
};

