
#include "MasterRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

const float MasterRenderer::s_fov = 70.f;
const float MasterRenderer::s_near_plane = 0.1f;
const float MasterRenderer::s_far_plane = 1000.f;
glm::mat4 MasterRenderer::s_projection_matrix = glm::mat4(0);

MasterRenderer::MasterRenderer()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	enableCulling();

	m_entity_shader = std::make_shared<DefaultShader>();
	m_terrain_shader = std::make_shared<TerrainShader>();
	m_entity_renderer = EntityRenderer(m_entity_shader);
	m_terrain_renderer = TerrainRenderer(m_terrain_shader);

	m_entity_shader->setUp();
	m_terrain_shader->setUp();
}

void MasterRenderer::render(const Light& sun)
{
	prepare();

	// entity shader
	m_entity_shader->start();
	// Loading some uniforms
	m_entity_shader->loadLight(sun);
	m_entity_shader->loadViewProjection();
	m_entity_renderer.render(m_entities);
	m_entity_shader->stop();

	// terrain shader
	m_terrain_shader->start();
	m_terrain_shader->loadLight(sun);
	m_terrain_shader->loadViewProjection();
	m_terrain_renderer.render(m_terrains);
	m_terrain_shader->stop();

	m_terrains.clear();
	m_entities.clear();
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set viewport to entire window 
	int width, height;
	glfwGetWindowSize(Application::s_window, &width, &height);
	glViewport(0, 0, width, height);

	// Setting the projection matrix
	float aspect_ratio = (float) width / height;
	s_projection_matrix = glm::perspective(s_fov, aspect_ratio, s_near_plane, s_far_plane);
}

void MasterRenderer::processEntity(const Entity& entity)
{
	const TexturedModel& model = entity.getModel();
	if (m_entities.find(model) == m_entities.end()) {
		std::vector<Entity> list;
		m_entities.insert({ model, list });
	}
	m_entities.at(model).push_back(entity);
}

void MasterRenderer::processTerrain(const Terrain& terrain)
{
	m_terrains.push_back(terrain);
}