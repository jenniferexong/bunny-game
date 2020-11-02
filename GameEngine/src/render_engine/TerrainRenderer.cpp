
#include <gl/glew.h>

#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer(std::shared_ptr<TerrainShader> shader) : m_shader(shader)
{
	m_shader->connectTextureUnits();
}

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
	const TerrainTexture texture = terrain.getTexture();

	// Render the mesh
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(ePosition);
	glEnableVertexAttribArray(eNormal);
	glEnableVertexAttribArray(eTexture);

	// Binding textures
	bindTextures(terrain);
	m_shader->loadMaterial(texture.getMaterial());
}

void TerrainRenderer::bindTextures(const Terrain& terrain) {
	std::shared_ptr<TerrainTexturePack> texture_pack = terrain.getTexture().getTexturePack();
	// base
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getBase().getId());
	// red
	glActiveTexture(GL_TEXTURE0 + eRed);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getRed().getId());
	// green
	glActiveTexture(GL_TEXTURE0 + eGreen);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getGreen().getId());
	// blue
	glActiveTexture(GL_TEXTURE0 + eBlue);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getBlue().getId());
	// blend map
	glActiveTexture(GL_TEXTURE0 + eBlendMap);
	glBindTexture(GL_TEXTURE_2D, terrain.getTexture().getBlendMap().getId());
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
