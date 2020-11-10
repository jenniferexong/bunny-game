
#include "MasterRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

const float MasterRenderer::fov = 70.f;
const float MasterRenderer::near_plane = 0.1f;
const float MasterRenderer::far_plane = 1000.f;
glm::mat4 MasterRenderer::projection_matrix = glm::mat4(0);

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

void MasterRenderer::render(const std::vector<Light>& lights)
{
	prepare();

	// terrain 
	terrain_shader_->start();
	terrain_shader_->loadUniformPerFrame(lights);
	terrain_renderer_.render(terrains_);
	terrain_shader_->stop();

	// entities
	entity_shader_->start();
	// Loading some uniforms
	entity_shader_->loadUniformPerFrame(lights);
	//entity_renderer_.render(entities_);
	entity_renderer_.renderInstanced(entities_);
	entity_shader_->stop();

	gui_renderer_.render(guis_);

	terrains_.clear();
	entities_.clear();
	guis_.clear();
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
void MasterRenderer::prepare()
{
	glm::vec3 sky = Application::sky_color;
	glClearColor(sky.x, sky.y, sky.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set viewport to entire window 
	int width, height;
	glfwGetWindowSize(Application::window, &width, &height);
	glViewport(0, 0, width, height);

	// Setting the projection matrix
	float aspect_ratio = (float) width / height;
	projection_matrix = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
}

void MasterRenderer::processEntity(const Entity& entity)
{
	const TexturedModel& model = entity.getModel();
	if (entities_.find(model) == entities_.end()) {
		std::vector<Entity> list;
		entities_.insert({ model, list });
	}
	entities_.at(model).push_back(entity);
}

void MasterRenderer::processTerrain(const Terrain& terrain)
{
	terrains_.push_back(terrain);
}

void MasterRenderer::processGui(const std::shared_ptr<GuiTexture>& gui)
{
	guis_.push_back(gui);
}