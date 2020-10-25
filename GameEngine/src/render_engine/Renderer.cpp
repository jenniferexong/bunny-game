
#include <gl/glew.h>

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Entity.h"
#include "../AttributeLocation.h"
#include "../shaders/Shader.h"

/* Setting the constants */
const float Renderer::s_fov = 70.f;
const float Renderer::s_near_plane = 0.1f;
const float Renderer::s_far_plane = 1000.f;

/* Clears the window */
void Renderer::prepare() {
	glClearColor(0.f, 0.f, 0.f, 0.f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const Entity& entity, const Shader& shader) {
	const Mesh mesh = entity.getModel().getMesh();
	const ModelTexture texture = entity.getModel().getTexture();

	// Render the mesh
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glEnableVertexAttribArray(eTexture);

	// Loading uniform variables
	shader.loadAllUniforms(getProjectionMatrix(), entity);
	
	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());

	glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 
}

glm::mat4 Renderer::getProjectionMatrix() {
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	float aspect_ratio = (float) width / height;

	return glm::perspective(s_fov, aspect_ratio, s_near_plane, s_far_plane);
}
