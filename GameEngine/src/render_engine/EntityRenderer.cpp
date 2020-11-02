
#include <gl/glew.h>

#include "EntityRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "../AttributeLocation.h"
#include "../objects/Camera.h"
#include "../Application.h"

using std::cout;
using std::endl;


void EntityRenderer::render(std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities)
{
	for (auto const& element : entities) {
		const TexturedModel& model = element.first;
		prepareTexturedModel(model);
		const std::vector<Entity>& batch = entities.at(model);
		for (const Entity &entity : batch) {
			loadTransformation(entity);
			glDrawElements(GL_TRIANGLES, model.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);
		}
		unbindTexturedModel();
	}
}

void EntityRenderer::prepareTexturedModel(const TexturedModel& model)
{
	const Mesh mesh = model.getMesh();
	const ModelTexture texture = model.getTexture();

	if (texture.getMaterial().has_transparency) {
		MasterRenderer::disableCulling();
	}

	// Render the mesh
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glEnableVertexAttribArray(eNormal);
	glEnableVertexAttribArray(eTexture);
	
	m_shader->loadMaterial(texture.getMaterial());

	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureId());
}

void EntityRenderer::unbindTexturedModel()
{
	MasterRenderer::enableCulling();
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eNormal);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 
}

void EntityRenderer::loadTransformation(const Entity& entity)
{
	m_shader->loadModelMatrix(entity);
}


