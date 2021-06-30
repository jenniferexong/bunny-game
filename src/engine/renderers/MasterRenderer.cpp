#include <GL/glew.h>

#include "MasterRenderer.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../shaders/EntityShader.h"
#include "../shaders/TerrainShader.h"
#include "../scene/Scene.h"
#include "../Camera.h"
#include "../environment/Environment.h"

glm::mat4 MasterRenderer::projection_matrix = glm::mat4(1);
float MasterRenderer::fov = 70.f;
float MasterRenderer::near_plane = 0.1f;
float MasterRenderer::far_plane = 1000.f;

using std::make_shared;

MasterRenderer::MasterRenderer()
{
	Log::init("MasterRenderer", false);

	glEnable(GL_MULTISAMPLE);
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

	Log::init("MasterRenderer", true);
}

void MasterRenderer::renderWaterReflection(WaterScene& scene)
{
	scene.getEnvironment().getCamera().lock()->positionForReflection(
		Water::height
	);
	water_renderer_.getReflectionFbo().bind();
    scene.renderScene(Water::getReflectionPlane());
	water_renderer_.getReflectionFbo().unbind();
}

void MasterRenderer::renderWaterRefraction(WaterScene& scene)
{
	scene.getEnvironment().getCamera().lock()->positionForRefraction(
		Water::height
	);
	water_renderer_.getRefractionFbo().bind();
    scene.renderScene(Water::getRefractionPlane());
	water_renderer_.getRefractionFbo().unbind();
}

void MasterRenderer::renderWater(const Environment& environment)
{
	water_renderer_.render(environment);
}

void MasterRenderer::renderShadowMap(const Environment& environment) 
{
	shadow_renderer_.render(environment);
}

void MasterRenderer::renderEntities(
	const Environment& environment,
	glm::vec4 clipping_plane)
{
	if (environment.getEntities().empty())
		return;

	entity_shader_->start();
	entity_shader_->loadUniformPerFrame(environment, clipping_plane);
	//entity_renderer_.render(environment);
	entity_renderer_.renderInstanced(environment);
	entity_shader_->stop();
}

void MasterRenderer::renderTerrain(
	const Environment& environment,
	glm::vec4 clipping_plane)
{
	if (environment.getTerrains().empty())
		return;

	terrain_shader_->start();
	terrain_shader_->loadUniformPerFrame(environment, clipping_plane);
	terrain_renderer_.render(environment.getTerrains());
	terrain_shader_->stop();
}

void MasterRenderer::renderSkybox(const Environment& environment)
{
	skybox_renderer_.render(environment);
}

void MasterRenderer::renderText(const TextMaster& text_master)
{
	text_renderer_.render(text_master);
}

void MasterRenderer::renderGui(const std::vector<weak_ptr<GuiTexture>>& guis)
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

void MasterRenderer::enableAlphaBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MasterRenderer::disableAlphaBlending()
{
	glDisable(GL_BLEND);
}

void MasterRenderer::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void MasterRenderer::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void MasterRenderer::resize(int width, int height)
{
	//water_renderer_.getReflectionFbo().resize(width, height);
	water_renderer_.getRefractionFbo().resize(width, height);
}

/* Clears the window */
void MasterRenderer::prepare(glm::mat4 proj_matrix)
{
	glClearColor(1, 1, 1, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection_matrix = proj_matrix;
}
