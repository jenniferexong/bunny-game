
#include <gl/glew.h>

#include "EntityRenderer.h"

#include <iostream>

#include "../Location.h"
#include "../Application.h"

using std::cout;
using std::endl;

void EntityRenderer::render(const std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities)
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

void EntityRenderer::renderInstanced(const std::map<TexturedModel, std::vector<Entity>, CompareTexturedModel>& entities)
{
	auto float_data = std::make_shared<vector<float>>();

	for (auto const& element : entities) {
		const TexturedModel& model = element.first;
		prepareTexturedModel(model);
		const std::vector<Entity>& batch = entities.at(model);

		Entity e = batch.at(0);
		int vao_id = e.getModel().getMesh().getId();

		float_data->clear();
		float_data->reserve(batch.size());

		loadTransformations(batch, float_data); // load all the transformation matrices into float_data

		// update vbo
		glBindBuffer(GL_ARRAY_BUFFER, e.getModel().getMesh().getModelMatrixVbo());
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * float_data->size(), float_data->data(), GL_STATIC_DRAW);

		if (e.getModel().getMesh().getFace() == 4) {
			glDrawElementsInstanced(GL_QUADS, model.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0, batch.size());
		}

		else if (e.getModel().getMesh().getFace() == 3) {
			glDrawElementsInstanced(GL_TRIANGLES, model.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0, batch.size());
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		unbindTexturedModel();
	}
}

void EntityRenderer::loadTransformations(const std::vector<Entity>& entities, std::shared_ptr<vector<float>> float_data)
{
	for (const auto& e: entities) {
		glm::mat4 t_matrix = Maths::createTransformationMatrix(e.getPosition(), 
			e.getActualRotation(), e.getScale(), e.getAlignmentRotation());

		glm::vec4 col_1 = t_matrix[0];
		glm::vec4 col_2 = t_matrix[1];
		glm::vec4 col_3 = t_matrix[2];
		glm::vec4 col_4 = t_matrix[3];
		
		float_data->push_back(col_1.x);
		float_data->push_back(col_1.y);
		float_data->push_back(col_1.z);
		float_data->push_back(col_1.w);

		float_data->push_back(col_2.x);
		float_data->push_back(col_2.y);
		float_data->push_back(col_2.z);
		float_data->push_back(col_2.w);

		float_data->push_back(col_3.x);
		float_data->push_back(col_3.y);
		float_data->push_back(col_3.z);
		float_data->push_back(col_3.w);

		float_data->push_back(col_4.x);
		float_data->push_back(col_4.y);
		float_data->push_back(col_4.z);
		float_data->push_back(col_4.w);
	}
}

void EntityRenderer::prepareTexturedModel(const TexturedModel& model)
{
	const InstancedMesh mesh = model.getMesh();
	const ModelTexture texture = model.getTexture();

	if (texture.getMaterial().has_transparency) {
		MasterRenderer::disableCulling();
	}

	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);
	glEnableVertexAttribArray(AttributeLocation::Normal);
	glEnableVertexAttribArray(AttributeLocation::Texture);
	// model matrices
	glEnableVertexAttribArray(AttributeLocation::ModelMatrixColumn1);
	glEnableVertexAttribArray(AttributeLocation::ModelMatrixColumn2);
	glEnableVertexAttribArray(AttributeLocation::ModelMatrixColumn3);
	glEnableVertexAttribArray(AttributeLocation::ModelMatrixColumn4);
	
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
	glDisableVertexAttribArray(AttributeLocation::ModelMatrixColumn1);
	glDisableVertexAttribArray(AttributeLocation::ModelMatrixColumn2);
	glDisableVertexAttribArray(AttributeLocation::ModelMatrixColumn3);
	glDisableVertexAttribArray(AttributeLocation::ModelMatrixColumn4);
	glBindVertexArray(0); // unbind 
}

void EntityRenderer::loadTransformation(const Entity& entity)
{
	shader_->loadModelMatrix(entity);
}


