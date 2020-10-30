
#include "MasterRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Application.h"

const float MasterRenderer::s_fov = 70.f;
const float MasterRenderer::s_near_plane = 0.1f;
const float MasterRenderer::s_far_plane = 1000.f;
glm::mat4 MasterRenderer::s_projection_matrix = glm::mat4(0);

MasterRenderer::MasterRenderer(const DefaultShader& shader)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

	m_shader = std::make_shared<DefaultShader>(shader);
	m_renderer = EntityRenderer(m_shader);

	m_shader->setUp();
}

void MasterRenderer::render(const Light& sun)
{
	prepare();
	m_shader->start();

	// Loading some uniforms
	m_shader->loadLight(sun);
	m_shader->loadViewProjection();

	m_renderer.render(m_entities);

	m_shader->stop();
	m_entities.clear();
}

/* Clears the window */
void MasterRenderer::prepare()
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.f); 
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