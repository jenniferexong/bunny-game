#pragma once

#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "GuiRenderer.h"
#include "SkyboxRenderer.h"
#include "TextRenderer.h"
#include "WaterRenderer.h"
#include "ShadowRenderer.h"
#include "../post-processing/PostProcessor.h"

class Scene;
class WaterScene;

using std::shared_ptr;

class MasterRenderer {
private:
	shared_ptr<EntityShader> entity_shader_;
	shared_ptr<TerrainShader> terrain_shader_;

	EntityRenderer entity_renderer_;
	TerrainRenderer terrain_renderer_;
	SkyboxRenderer skybox_renderer_;
	WaterRenderer water_renderer_;
	ShadowRenderer shadow_renderer_;

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

	void renderShadowMap(const Environment& environment);
	void renderEntities(
		const Environment& environment,
		glm::vec4 clipping_plane
	);
	void renderTerrain(
		const Environment& environment,
		glm::vec4 clipping_plane
	);
	void renderSkybox(const Environment& environment);

	void renderWater(const Environment& environment);
	void renderWaterReflection(WaterScene& scene);
	void renderWaterRefraction(WaterScene& scene);

	void renderGui(const std::vector<weak_ptr<GuiTexture>>& guis);
	void renderText(const TextMaster& text_master);

	int getShadowMapTexture() const
	{
		return shadow_renderer_.getShadowMapTexture();
	}

	static void enableCulling();
	static void disableCulling();
	static void enableAlphaBlending();
	static void disableAlphaBlending();
	static void enableDepthTest();
	static void disableDepthTest();
};

