#pragma once

#include <memory>
#include <vector>

#include "../shaders/TerrainShader.h"
#include "../objects/Terrain.h"

class TerrainRenderer {
private:
	std::shared_ptr<TerrainShader> _shader;

	void prepareTerrain(const Terrain& terrain);
	void unbindTerrain();
	void bindTextures(const Terrain& terrain);
	void loadTransformation(const Terrain& terrain);

public:
	TerrainRenderer(): _shader(nullptr) {}
	TerrainRenderer(std::shared_ptr<TerrainShader> shader);

	void render(const std::vector<Terrain>& terrains);
};
