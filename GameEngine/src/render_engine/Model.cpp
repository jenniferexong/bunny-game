
#include <GL/glew.h>

#include "Model.h"

#include "../AttributeLocation.h"

void Model::render() const {
	// Render the mesh
	printf("r\n");
	glBindVertexArray(m_mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glDrawElements(GL_TRIANGLES, m_mesh.getVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(ePosition);
	glBindVertexArray(0); // unbind 
}

void TexturedModel::render() const {
	// Make the shader use the texture

	printf("rendering\n");

	// Render the mesh
	glBindVertexArray(m_mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glEnableVertexAttribArray(eTexture);
	
	// texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture.getId());

	glDrawElements(GL_TRIANGLES, m_mesh.getVertexCount(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 
}
