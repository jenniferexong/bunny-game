
#include <gl/glew.h>

#include "EntityRenderer.h"

#include <iostream>

#include "../Location.h"
#include "../Application.h"

using std::cout;
using std::endl;

void EntityRenderer::render(const map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, CompareTexturedModel> & entities)
{
	for (const auto& element : entities) {
		const auto& model = element.first;
		prepareTexturedModel(*model);
		const auto& batch = element.second;
		for (const auto &entity : *batch) {
			loadTransformation(*entity);

			if (model->getMesh().getFace() == 4)
				glDrawElements(GL_QUADS, model->getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);

			else if (model->getMesh().getFace() == 3)
				glDrawElements(GL_TRIANGLES, model->getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);
		}
		unbindTexturedModel();
	}
}

void EntityRenderer::renderInstanced(const map<shared_ptr<TexturedModel>, shared_ptr<set<shared_ptr<Entity>>>, CompareTexturedModel>& entities)
{
	auto model_matrix_data = std::make_shared<vector<float>>();
	auto model_brightness_data = std::make_shared<vector<float>>();

	for (const auto& element : entities) {
		const auto& model = element.first;
		prepareTexturedModel(*model);
		const auto& batch = element.second;

		int vao_id = model->getMesh().getId();

		model_matrix_data->clear();
		model_matrix_data->reserve(batch->size());
		model_brightness_data->clear();
		model_brightness_data->reserve(batch->size());

		loadTransformations(*batch, model_matrix_data); // load all the transformation matrices into float_data
		loadColors(*batch, model_brightness_data); // load all the transformation matrices into float_data

		// update vbos
		glBindBuffer(GL_ARRAY_BUFFER, model->getMesh().getModelMatrixVbo());
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model_matrix_data->size(), model_matrix_data->data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// colors
		glBindBuffer(GL_ARRAY_BUFFER, model->getMesh().getModelBrightnessVbo());
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model_brightness_data->size(), model_brightness_data->data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (model->getMesh().getFace() == 4) {
			glDrawElementsInstanced(GL_QUADS, model->getMesh().getVertexCount(), GL_UNSIGNED_INT, 0, batch->size());
		}

		else if (model->getMesh().getFace() == 3) {
			glDrawElementsInstanced(GL_TRIANGLES, model->getMesh().getVertexCount(), GL_UNSIGNED_INT, 0, batch->size());
		}

		unbindTexturedModel();
	}
}

void EntityRenderer::loadColors(const std::set<shared_ptr<Entity>>& entities, std::shared_ptr<vector<float>> float_data)
{
	for (const auto& e : entities) {
		float brightness = e->getBrightness();
		float_data->push_back(brightness);
	}
}

void EntityRenderer::loadTransformations(const std::set<shared_ptr<Entity>>& entities, std::shared_ptr<vector<float>> float_data)
{
	for (const auto& e: entities) {
		glm::mat4 t_matrix = Maths::createTransformationMatrix(e->getPosition(), 
			e->getActualRotation(), e->getScale(), e->getAlignmentRotation());

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
	// color
	glEnableVertexAttribArray(AttributeLocation::ModelBrightness);
	
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
	glDisableVertexAttribArray(AttributeLocation::ModelBrightness);
	glBindVertexArray(0); // unbind 
}

void EntityRenderer::loadTransformation(const Entity& entity)
{
	shader_->loadModelMatrix(entity);
}


