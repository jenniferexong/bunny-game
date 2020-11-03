
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
	enableCulling();

	_entity_shader = std::make_shared<DefaultShader>();
	_terrain_shader = std::make_shared<TerrainShader>();
	_entity_shader->setUp();
	_terrain_shader->setUp();

	_entity_renderer = EntityRenderer(_entity_shader);
	_terrain_renderer = TerrainRenderer(_terrain_shader);
}

void MasterRenderer::render(const Light& sun)
{
	prepare();

	// entity shader
	_entity_shader->start();
	// Loading some uniforms
	_entity_shader->loadUniformPerFrame(sun);
	_entity_renderer.render(_entities);
	_entity_shader->stop();

	// terrain shader
	_terrain_shader->start();
	_terrain_shader->loadUniformPerFrame(sun);
	_terrain_renderer.render(_terrains);
	_terrain_shader->stop();

	_terrains.clear();
	_entities.clear();
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
	if (_entities.find(model) == _entities.end()) {
		std::vector<Entity> list;
		_entities.insert({ model, list });
	}
	_entities.at(model).push_back(entity);
}

void MasterRenderer::processTerrain(const Terrain& terrain)
{
	_terrains.push_back(terrain);
}