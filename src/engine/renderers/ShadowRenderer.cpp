#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "ShadowRenderer.h"
#include "MasterRenderer.h"
#include "../Utility.h"
#include "../Location.h"
#include "../Environment.h"

const int ShadowRenderer::shadow_map_size = 4096;

ShadowRenderer::ShadowRenderer()
{
	shader_.setUp();
}

using namespace std;
void ShadowRenderer::render(const Environment& environment)
{
	vec3 light_direction = -environment.getSun().lock()->getPosition();
	shadow_box_.update(*environment.getCamera().lock(), light_view_matrix_);
	prepare(light_direction, *environment.getCamera().lock());
	renderEntities(environment);
	renderTerrain(environment);
	finish();
	Error::gl_check("ShadowRenderer");
}

void ShadowRenderer::prepare(vec3 light_direction, const Camera& camera)
{
	// update orthographic projection
	projection_matrix_ = glm::ortho(
		shadow_box_.getBoundsX().x, shadow_box_.getBoundsX().y,
		shadow_box_.getBoundsY().x, shadow_box_.getBoundsY().y,
		shadow_box_.getBoundsZ().x, shadow_box_.getBoundsZ().y
	);

	// View matrix is centered at the center of the cuboid.
	vec3 position = shadow_box_.getCenter();
	vec3 look_position = position + glm::normalize(light_direction);
	light_view_matrix_ = glm::lookAt(
		shadow_box_.getCenter(),
		look_position,
		vec3(0, 1.f, 0)
	); 

	fbo_.bind();
	glEnable(GL_DEPTH_TEST);
	MasterRenderer::enableCulling();
	glClear(GL_DEPTH_BUFFER_BIT);
	shader_.start();
}

void ShadowRenderer::updateOrthoProjection(float width, float height, float length)
{
	projection_matrix_ = mat4(1.f);
	projection_matrix_[0][0] = 2.f / width;
	projection_matrix_[1][1] = 2.f / height;
	projection_matrix_[2][2] = -2.f / length;
	projection_matrix_[3][3] = 1.f;
}

glm::mat4 ShadowRenderer::createOffset()
{
	using glm::mat4;
	mat4 offset = mat4(1.f);
	glm::translate(offset, vec3(0.5f));
	glm::scale(offset, vec3(0.5f));
	return offset;
}

/*
 * Creates a transformation matrix to align the 
 * orthographic cuboid with the light's direction.
 * Determines how the cuboid should be positioned in the world.
 */
void ShadowRenderer::updateLightView(vec3 direction, vec3 center)
{
	// direction already normalized
	vec3 n_center = -center;
	light_view_matrix_ = mat4(1.f);

	float pitch = glm::acos(length(vec2(direction.x, direction.z)));
	light_view_matrix_ = glm::rotate(light_view_matrix_, pitch, vec3(1.f, 0, 0));

	float yaw = glm::degrees(glm::atan(direction.x / direction.z));
	yaw = direction.z > 0 ? yaw - 180 : yaw;
	light_view_matrix_ = glm::rotate(
		light_view_matrix_, glm::radians(yaw), vec3(0, 1.f, 0));
	light_view_matrix_ = glm::translate(light_view_matrix_, n_center);
	Log::matrix("updated light view", light_view_matrix_);
}

void ShadowRenderer::finish()
{
	shader_.stop();
	fbo_.unbind();
}

void ShadowRenderer::renderEntities(const Environment& environment)
{
	/*
	const auto& entities = environment.getEntities();
	for (const auto& e: entities) {
		const auto& model = e->getModel();
		InstancedMesh mesh = model.getMesh();
		bindMesh(mesh);
		prepareInstance(*e);
		glDrawElements( GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(AttributeLocation::Position);
		glBindVertexArray(0);
	}
	*/
	const auto& entities = environment.getEntitiesInView();
	for (const auto& element : entities) {
		const auto& model = element.first;
		InstancedMesh mesh = model.getMesh();
		bindMesh(mesh);
		for (const auto& entity : element.second) {
			prepareInstance(*entity.lock());
			glDrawElements( GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, 0);
		}
		glDisableVertexAttribArray(AttributeLocation::Position);
		glBindVertexArray(0);
	}
}

void ShadowRenderer::renderTerrain(const Environment& environment)
{
	for (const Terrain& terrain : environment.getTerrains()) {
		bindMesh(terrain.getMesh());

		glm::vec3 position(terrain.getX(), 0, terrain.getZ());
		glm::mat4 t_matrix = Maths::createTransformationMatrix(
			position, glm::vec3(0), 1, glm::mat4(1));
		glm::mat4 matrix = getLightSpaceMatrix() * t_matrix;
		shader_.loadUniforms(matrix);

		glDrawElements(
			GL_TRIANGLES, terrain.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(AttributeLocation::Position);
		glBindVertexArray(0);
	}
}

void ShadowRenderer::bindMesh(const Mesh& mesh)
{
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);
}

void ShadowRenderer::prepareInstance(const Entity& entity)
{
	glm::mat4 t_matrix = entity.getModelMatrix();
	glm::mat4 matrix = getLightSpaceMatrix() * t_matrix;
	shader_.loadUniforms(matrix);
}

glm::mat4 ShadowRenderer::getLightSpaceMatrix()
{
	return projection_matrix_ * light_view_matrix_;
}
