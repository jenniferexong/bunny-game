#include <gl/glew.h>

#include "TerrainRenderer.h"
#include "../Location.h"
#include "../shaders/TerrainShader.h"
#include "../models/Mesh.h"
#include "../models/Texture.h"
#include "../../environment/Terrain.h"
#include "../Utility.h"

TerrainRenderer::TerrainRenderer(std::shared_ptr<TerrainShader> shader) : shader_(std::move(shader))
{
	shader_->start();
	shader_->connectTextureUnits();
	shader_->stop();
}

void TerrainRenderer::render(const std::vector<Terrain>& terrains)
{
	for (const Terrain& terrain : terrains) {
		prepareTerrain(terrain);
		loadTransformation(terrain);
		glDrawElements(GL_TRIANGLES, terrain.getMesh().getVertexCount(), GL_UNSIGNED_INT, 0);
		unbindTerrain();
	}
	Error::gl_check(name_);
}

void TerrainRenderer::prepareTerrain(const Terrain& terrain)
{
	const Mesh mesh = terrain.getMesh();
	const TerrainTexture texture = terrain.getTexture();

	// Render the mesh
	glBindVertexArray(mesh.getId());
	glEnableVertexAttribArray(TerrainAttributeLocation::Position);
	glEnableVertexAttribArray(TerrainAttributeLocation::Normal);
	glEnableVertexAttribArray(TerrainAttributeLocation::Texture);
	glEnableVertexAttribArray(TerrainAttributeLocation::Tangent);

	// Binding textures
	shader_->loadMaterial(texture.getMaterial());
	bindTextures(terrain);
}

void TerrainRenderer::bindTextures(const Terrain& terrain) {
	std::shared_ptr<TerrainTexturePack> texture_pack = terrain.getTexture().getTexturePack();

	glActiveTexture(GL_TEXTURE0 + TextureLocation::Base);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getBase().getId());
	// red
	glActiveTexture(GL_TEXTURE0 + TextureLocation::Red);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getRed().getId());
	// green
	glActiveTexture(GL_TEXTURE0 + TextureLocation::Green);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getGreen().getId());
	// blue
	glActiveTexture(GL_TEXTURE0 + TextureLocation::Blue);
	glBindTexture(GL_TEXTURE_2D, texture_pack->getBlue().getId());
	// blend map
	glActiveTexture(GL_TEXTURE0 + TextureLocation::BlendMap);
	glBindTexture(GL_TEXTURE_2D, terrain.getTexture().getBlendMap().getId());
	// normal map
	glActiveTexture(GL_TEXTURE0 + TextureLocation::NormalMap);
	glBindTexture(GL_TEXTURE_2D, terrain.getTexture().getNormalMap().getId());
}

void TerrainRenderer::unbindTerrain()
{
	glDisableVertexAttribArray(TerrainAttributeLocation::Position);
	glDisableVertexAttribArray(TerrainAttributeLocation::Normal);
	glDisableVertexAttribArray(TerrainAttributeLocation::Texture);
	glDisableVertexAttribArray(TerrainAttributeLocation::Tangent);
	glBindVertexArray(0); // unbind 
}

void TerrainRenderer::loadTransformation(const Terrain& terrain)
{
	shader_->loadModelMatrix(terrain);
}
