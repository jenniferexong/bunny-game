
#include <gl/glew.h>

#include "TerrainRenderer.h"

void TerrainRenderer::render(const std::vector<Terrain>& terrains)
{
	for (const Terrain& terrain : terrains) {
		prepareTerrain(terrain);
		loadTransformation(terrain);
		glDrawElements(GL_TRIANGLES, terrain.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);
		unbindTerrain();
	}
}

void TerrainRenderer::prepareTerrain(const Terrain& terrain)
{
	const Mesh mesh = terrain.getMesh();
	const ModelTexture texture = terrain.getTexture();

	// Render the mesh
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glEnableVertexAttribArray(eNormal);
	glEnableVertexAttribArray(eTexture);

	m_shader->loadMaterial(texture);

	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());

	// Make the shader use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());
}

void TerrainRenderer::unbindTerrain()
{
	glDisableVertexAttribArray(ePosition);
	glDisableVertexAttribArray(eNormal);
	glDisableVertexAttribArray(eTexture);
	glBindVertexArray(0); // unbind 
}

void TerrainRenderer::loadTransformation(const Terrain& terrain)
{
	m_shader->loadModelMatrix(terrain);
}
