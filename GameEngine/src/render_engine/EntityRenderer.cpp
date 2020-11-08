
#include <gl/glew.h>

#include "EntityRenderer.h"

#include <iostream>

#include "../Location.h"
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

			if (entity.getModel().getMesh().getFace() == 4)
				glDrawElements(GL_QUADS, model.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);

			else if (entity.getModel().getMesh().getFace() == 3)
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
	glEnableVertexAttribArray(AttributeLocation::Position);
	glEnableVertexAttribArray(AttributeLocation::Normal);
	glEnableVertexAttribArray(AttributeLocation::Texture);
	
	shader_->loadMaterial(texture.getMaterial());

	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

}

void EntityRenderer::unbindTexturedModel()
{
	MasterRenderer::enableCulling();
	glDisableVertexAttribArray(AttributeLocation::Position);
	glDisableVertexAttribArray(AttributeLocation::Normal);
	glDisableVertexAttribArray(AttributeLocation::Texture);
	glBindVertexArray(0); // unbind 
}

void EntityRenderer::loadTransformation(const Entity& entity)
{
	shader_->loadModelMatrix(entity);
}


