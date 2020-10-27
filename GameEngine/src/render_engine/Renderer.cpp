
#include <gl/glew.h>

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Entity.h"
#include "../AttributeLocation.h"
#include "../shaders/Shader.h"
#include "../Application.h"
#include "Camera.h"

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
	glDepthFunc(GL_LESS);

	// Set viewport to entire window 
	int width, height;
	glfwGetWindowSize(Application::s_window, &width, &height);
	glViewport(0, 0, width, height);

	// Setting the projection matrix
	float aspect_ratio = (float) width / height;
	s_projection_matrix = glm::perspective(s_fov, aspect_ratio, s_near_plane, s_far_plane);
}

void Renderer::render(const Entity& entity, const Shader& shader) {
	const Mesh mesh = entity.getModel().getMesh();
	const ModelTexture texture = entity.getModel().getTexture();

	// Render the mesh
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glEnableVertexAttribArray(eNormal);
	glEnableVertexAttribArray(eTexture);

	// Loading uniform variable
	shader.loadAllUniforms(entity);
	
	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());

	glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eNormal);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 
}
