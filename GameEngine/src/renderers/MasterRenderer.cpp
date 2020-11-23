
#include "MasterRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "../Application.h"
#include "../scene/GameScene.h"

glm::mat4 MasterRenderer::projection_matrix = glm::mat4(1);
float MasterRenderer::fov = 70.f;
float MasterRenderer::near_plane = 0.1f;
float MasterRenderer::far_plane = 1000.f;

int MasterRenderer::window_width = 1280;
int MasterRenderer::window_height = 720;

MasterRenderer::MasterRenderer()
{
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	enableCulling();

	entity_shader_ = std::make_shared<EntityShader>();
	terrain_shader_ = std::make_shared<TerrainShader>();
	entity_shader_->setUp();
	terrain_shader_->setUp();

	entity_renderer_ = EntityRenderer(entity_shader_);
	terrain_renderer_ = TerrainRenderer(terrain_shader_);
}

void MasterRenderer::renderWaterReflection(GameScene& scene, void(GameScene::*render_scene)(glm::vec4, bool))
{
	scene.getEnvironment().getCamera()->positionForReflection(-15.f);
	water_fbos_.bindReflectionFrameBuffer();
	(scene.*render_scene)(Water::getReflectionPlane(), false);
	water_fbos_.unbindCurrentFrameBuffer();
}

void MasterRenderer::renderWaterRefraction(GameScene& scene, void(GameScene::*render_scene)(glm::vec4, bool))
{
	scene.getEnvironment().getCamera()->positionForRefraction(-15.f);
	water_fbos_.bindRefractionFrameBuffer();
	(scene.*render_scene)(Water::getRefractionPlane(), false);
	water_fbos_.unbindCurrentFrameBuffer();
}

void MasterRenderer::renderWater(const Environment& environment)
{
	water_renderer_.render(environment);
}

void MasterRenderer::renderEntities(const Environment& environment, glm::vec4 clipping_plane, bool progress_time)
{
	if (environment.getEntities().empty())
		return;

	entity_shader_->start();
	entity_shader_->loadUniformPerFrame(environment, clipping_plane);
	//entity_renderer_.render(entities);
	entity_renderer_.renderInstanced(environment);
	entity_shader_->stop();
}

void MasterRenderer::renderTerrain(const Environment& environment, glm::vec4 clipping_plane, bool progress_time)
{
	if (environment.getTerrains().empty())
		return;

	terrain_shader_->start();
	terrain_shader_->loadUniformPerFrame(environment, clipping_plane);
	terrain_renderer_.render(environment.getTerrains());
	terrain_shader_->stop();
}

void MasterRenderer::renderSkybox(const Environment& environment, bool progress_time)
{
	skybox_renderer_.render(environment, progress_time);
}

void MasterRenderer::renderGui(const vector<shared_ptr<GuiTexture>>& guis)
{
	gui_renderer_.render(guis);
}

void MasterRenderer::enableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MasterRenderer::disableCulling()
{
	glDisable(GL_CULL_FACE);
}

/* Clears the window */
void MasterRenderer::prepare(glm::mat4 proj_matrix)
{
	glClearColor(1, 1, 1, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glViewport(0, 0, window_width, window_height);

	projection_matrix = proj_matrix;
}
