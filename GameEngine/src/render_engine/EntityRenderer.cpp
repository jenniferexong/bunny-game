
#include <gl/glew.h>

#include "EntityRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "../AttributeLocation.h"
#include "../object/Camera.h"
#include "../Application.h"

using std::cout;
using std::endl;

/* Setting the constants */

/* Clears the window */
void EntityRenderer::prepare()
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
}

void EntityRenderer::render(std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities)
{
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

void EntityRenderer::prepareTexturedModel(const TexturedModel& model)
{
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

void EntityRenderer::unbindTexturedModel()
{
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eNormal);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 
}

void EntityRenderer::prepareInstance(const Entity& entity)
{
	m_shader->loadModelMatrix(entity);
}


