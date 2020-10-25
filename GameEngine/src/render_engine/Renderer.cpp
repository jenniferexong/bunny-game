
#include <gl/glew.h>

#include "../Entity.h"
#include "../AttributeLocation.h"
#include "Renderer.h"
#include "../shaders/Shader.h"

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
	shader.loadAllUniforms(entity);
	
	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());

	glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 

}
