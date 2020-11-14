
#include "MasterRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

glm::mat4 MasterRenderer::projection_matrix = glm::mat4(1);

MasterRenderer::MasterRenderer()
{
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
	renderScene(scene);
	gui_renderer_.render(scene->getGuis());
}

void MasterRenderer::renderScene(const shared_ptr<Scene>& scene)
{
	render(scene);
}

void MasterRenderer::render(const shared_ptr<Scene>& scene)
{
	prepare(scene->getProjectionMatrix());

	// entities
	if (!scene->getEnvironment().getEntities().empty()) {
		entity_shader_->start();
		// Loading some uniforms
		entity_shader_->loadUniformPerFrame(scene->getEnvironment());
		//entity_renderer_.render(entities);
		entity_renderer_.renderInstanced(scene->getEnvironment());
		entity_shader_->stop();
	}

	// terrain 
	if (!scene->getEnvironment().getTerrains().empty()) {
		terrain_shader_->start();
		terrain_shader_->loadUniformPerFrame(scene->getEnvironment());
		terrain_renderer_.render(scene->getEnvironment().getTerrains());
		terrain_shader_->stop();
	}

	// skybox TOD0: put skybox into environment
	skybox_renderer_.render(scene->getEnvironment());
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

	// Set viewport to entire window 
	int width, height;
	glfwGetWindowSize(*Application::window, &width, &height);
	glViewport(0, 0, width, height);

	projection_matrix = proj_matrix;
}
