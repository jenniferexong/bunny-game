
#include <iostream>
#include <gl/glew.h>

#include "TerrainRenderer.h"
#include "../Location.h"

TerrainRenderer::TerrainRenderer(std::shared_ptr<TerrainShader> shader) : _shader(std::move(shader))
{
	_shader->connectTextureUnits();
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
	glEnableVertexAttribArray(AttributeLocation::Position);
	glEnableVertexAttribArray(AttributeLocation::Normal);
	glEnableVertexAttribArray(AttributeLocation::Texture);

	// Binding textures
	_shader->loadMaterial(texture.getMaterial());
	bindTextures(terrain);
}

void TerrainRenderer::bindTextures(const Terrain& terrain) {
	std::shared_ptr<TerrainTexturePack> texture_pack = terrain.getTexture().getTexturePack();
	// base
	int base = texture_pack->getBase().getId();
	int red = texture_pack->getRed().getId();
	int green = texture_pack->getGreen().getId();
	int blue = texture_pack->getBlue().getId();
	int blend = terrain.getTexture().getBlendMap().getId();
	std::cout << "base: " << base << std::endl;
	std::cout << "red: " << red << std::endl;
	std::cout << "green: " << green << std::endl;
	std::cout << "blue: " << blue << std::endl;
	std::cout << "blend: " << blend << std::endl;

	glActiveTexture(GL_TEXTURE0);
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
}

void TerrainRenderer::unbindTerrain()
{
	glDisableVertexAttribArray(AttributeLocation::Position);
	glDisableVertexAttribArray(AttributeLocation::Normal);
	glDisableVertexAttribArray(AttributeLocation::Texture);
	glBindVertexArray(0); // unbind 
}

void TerrainRenderer::loadTransformation(const Terrain& terrain)
{
	_shader->loadModelMatrix(terrain);
}
