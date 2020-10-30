
#include <gl/glew.h>

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "../AttributeLocation.h"
#include "../object/Camera.h"
#include "../Application.h"

using std::cout;
using std::endl;

/* Setting the constants */
const float Renderer::s_fov = 70.f;
const float Renderer::s_near_plane = 0.1f;
const float Renderer::s_far_plane = 1000.f;
glm::mat4 Renderer::s_projection_matrix = glm::mat4(0);

/* Clears the window */
void Renderer::prepare() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

	// Set viewport to entire window 
	int width, height;
	glfwGetWindowSize(Application::s_window, &width, &height);
	glViewport(0, 0, width, height);

	// Setting the projection matrix
	float aspect_ratio = (float) width / height;
	s_projection_matrix = glm::perspective(s_fov, aspect_ratio, s_near_plane, s_far_plane);
}

void Renderer::render(std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities) {
	for (auto const& element : entities) {
		const TexturedModel& model = element.first;
		prepareTexturedModel(model);
		const std::vector<Entity>& batch = entities.at(model);
		for (const Entity &entity : batch) {
			prepareInstance(entity);
			glDrawElements(GL_TRIANGLES, model.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);
		}
		unbindTexturedModel();
	}
}

void Renderer::prepareTexturedModel(const TexturedModel& model) {
	const Mesh mesh = model.getMesh();
	const ModelTexture texture = model.getTexture();

	// Render the mesh
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glEnableVertexAttribArray(eNormal);
	glEnableVertexAttribArray(eTexture);
	
	m_shader->loadMaterial(texture);

	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());

	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());
}

void Renderer::unbindTexturedModel() {
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eNormal);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 
}

void Renderer::prepareInstance(const Entity& entity) {
	m_shader->loadModelMatrix(entity);
}

// Master Renderer
MasterRenderer::MasterRenderer(const DefaultShader& shader) {
	m_shader = std::make_shared<DefaultShader>(shader);
	m_renderer = Renderer(m_shader);

	m_shader->setUp();
}

void MasterRenderer::render(const Light& sun) {
	m_renderer.prepare();
	m_shader->start();

	// Loading some uniforms
	m_shader->loadLight(sun);
	m_shader->loadViewProjection();

	m_renderer.render(m_entities);

	m_shader->stop();
	m_entities.clear();
}

void MasterRenderer::processEntity(const Entity& entity) {
	const TexturedModel& model = entity.getModel();
	if (m_entities.find(model) == m_entities.end()) {
		std::vector<Entity> list;
		m_entities.insert({ model, list });
	}
	m_entities.at(model).push_back(entity);
}
