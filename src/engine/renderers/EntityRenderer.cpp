
#include <GL/glew.h>

#include "EntityRenderer.h"

#include <iostream>

#include "MasterRenderer.h"
#include "../Environment.h"
#include "../shaders/EntityShader.h"
#include "../Location.h"
#include "../Utility.h"

EntityRenderer::EntityRenderer(std::shared_ptr<EntityShader> shader): 
	shader_(std::move(shader))
{
	shader_->start();
	shader_->connectTextureUnits();
	shader_->stop();
}

void EntityRenderer::render(const Environment& environment)
{
	for (const auto& element : environment.getEntitiesInView()) {
		const auto& model = element.first;
		prepareTexturedModel(model);
		const auto& batch = element.second;
		for (const auto &entity : batch) {
			loadTransformation(*entity.lock());
			glDrawElements(
				GL_TRIANGLES,
				model.getMesh().getVertexCount(),
				GL_UNSIGNED_INT,
				0
			);
		}
		unbindTexturedModel();
	}
}

void EntityRenderer::renderInstanced(const Environment& environment)
{
	vector<float> model_matrix_data;
	vector<float> model_brightness_data;

	glActiveTexture(GL_TEXTURE0 + EntityTextureLocation::EnvironmentMap);
	glBindTexture(
		GL_TEXTURE_CUBE_MAP,
		environment.getSkybox().lock()->getDayTexture()
	);

	for (const auto& element : environment.getEntitiesInView()) {
		const auto& model = element.first;
		prepareTexturedModel(model);
		const auto& batch = element.second;

		model_matrix_data.clear();
		model_matrix_data.reserve(batch.size());
		model_brightness_data.clear();
		model_brightness_data.reserve(batch.size());

		// load all the transformation matrices into float_data
		loadTransformations(batch, model_matrix_data); 
		loadColors(batch, model_brightness_data); 

		// update vbos
		glBindBuffer(GL_ARRAY_BUFFER, model.getMesh().getModelMatrixVbo());
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(float) * model_matrix_data.size(),
			model_matrix_data.data(),
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// colors
		glBindBuffer(
			GL_ARRAY_BUFFER,
			model.getMesh().getModelBrightnessVbo()
		);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(float) * model_brightness_data.size(),
			model_brightness_data.data(), GL_STATIC_DRAW
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawElementsInstanced(
			GL_TRIANGLES,
			model.getMesh().getVertexCount(),
			GL_UNSIGNED_INT,
			0, batch.size()
		);
		unbindTexturedModel();
	}
	Error::gl_check("EntityRenderer renderInstanced");
}

void EntityRenderer::loadColors(
	const std::unordered_set<weak_ptr<Entity>>& entities,
	vector<float>& float_data)
{
	for (const auto& e : entities) {
		float brightness = e.lock()->getBrightness();
		float_data.push_back(brightness);
	}
}

void EntityRenderer::loadTransformations(
	const std::unordered_set<weak_ptr<Entity>>& entities,
	vector<float>& float_data)
{
	for (const auto& e: entities) {
		glm::mat4 t_matrix = e.lock()->getModelMatrix();

		glm::vec4 col_1 = t_matrix[0];
		glm::vec4 col_2 = t_matrix[1];
		glm::vec4 col_3 = t_matrix[2];
		glm::vec4 col_4 = t_matrix[3];
		
		float_data.push_back(col_1.x);
		float_data.push_back(col_1.y);
		float_data.push_back(col_1.z);
		float_data.push_back(col_1.w);

		float_data.push_back(col_2.x);
		float_data.push_back(col_2.y);
		float_data.push_back(col_2.z);
		float_data.push_back(col_2.w);

		float_data.push_back(col_3.x);
		float_data.push_back(col_3.y);
		float_data.push_back(col_3.z);
		float_data.push_back(col_3.w);

		float_data.push_back(col_4.x);
		float_data.push_back(col_4.y);
		float_data.push_back(col_4.z);
		float_data.push_back(col_4.w);
	}
}

void EntityRenderer::prepareTexturedModel(const TexturedModel& model)
{
	const InstancedMesh mesh = model.getMesh();
	const ModelTexture texture = model.getTexture();

	MasterRenderer::disableCulling();
	if (texture.getMaterial().has_transparency)
		MasterRenderer::disableCulling();

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

	glActiveTexture(GL_TEXTURE0 + EntityTextureLocation::DiffuseMap);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

	if (texture.getMaterial().hasGlowMap()) {
		glActiveTexture(GL_TEXTURE0 + EntityTextureLocation::GlowMap);
		glBindTexture(GL_TEXTURE_2D, texture.getMaterial().glow_map_);
	}

	Error::gl_check("EntityRenderer prepareTexturedModel");
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
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void EntityRenderer::loadTransformation(const Entity& entity)
{
	shader_->loadModelMatrix(entity);
}


