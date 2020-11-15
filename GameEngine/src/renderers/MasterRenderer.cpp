
#include "MasterRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

#include "../scene/GameScene.h"

glm::mat4 MasterRenderer::projection_matrix = glm::mat4(1);
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

void MasterRenderer::renderAll(const shared_ptr<Scene>& scene)
{
	int width, height;
	glfwGetFramebufferSize(*Application::window, &width, &height);
	window_width = width;
	window_height = height;

	// testing fbo:
	//GameScene::reflection_gui->setTexture(water_fbos_.getReflectionTexture());
	//GameScene::refraction_gui->setTexture(water_fbos_.getRefractionTexture());

	// render scene to fbos (not including water)
	// reflection
	scene->getEnvironment().getCamera()->positionForReflection(-15.f);
	water_fbos_.bindReflectionFrameBuffer();
	renderScene(scene, Water::getReflectionPlane(), false);
	water_fbos_.unbindCurrentFrameBuffer();

	// refraction
	scene->getEnvironment().getCamera()->positionForRefraction(-15.f);
	water_fbos_.bindRefractionFrameBuffer();
	renderScene(scene, Water::getRefractionPlane(), false);
	water_fbos_.unbindCurrentFrameBuffer();

	renderScene(scene, glm::vec4(0), true);
	water_renderer_.render(scene->getEnvironment());
	gui_renderer_.render(scene->getGuis());
}

void MasterRenderer::renderScene(const shared_ptr<Scene>& scene, glm::vec4 clipping_plane, bool progress_time)
{
	prepare(scene->getProjectionMatrix());

	// entities
	if (!scene->getEnvironment().getEntities().empty()) {
		entity_shader_->start();
		// Loading some uniforms
		entity_shader_->loadUniformPerFrame(scene->getEnvironment(), clipping_plane);
		//entity_renderer_.render(entities);
		entity_renderer_.renderInstanced(scene->getEnvironment());
		entity_shader_->stop();
	}

	// terrain 
	if (!scene->getEnvironment().getTerrains().empty()) {
		terrain_shader_->start();
		terrain_shader_->loadUniformPerFrame(scene->getEnvironment(), clipping_plane);
		terrain_renderer_.render(scene->getEnvironment().getTerrains());
		terrain_shader_->stop();
	}

	skybox_renderer_.render(scene->getEnvironment(), progress_time);
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
