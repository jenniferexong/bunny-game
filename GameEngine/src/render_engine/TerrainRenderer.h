#pragma once

#include <memory>
#include <vector>

#include "../shaders/TerrainShader.h"
#include "../objects/Terrain.h"
#include "../objects/Entity.h"
#include "../models/Model.h"

class TerrainRenderer {
private:
	std::shared_ptr<TerrainShader> m_shader;
	void prepareTerrain(const Terrain& terrain);
	void unbindTerrain();
	void bindTextures(const Terrain& terrain);
	void loadTransformation(const Terrain& terrain);

public:
	TerrainRenderer(): m_shader(nullptr) {}
	TerrainRenderer(std::shared_ptr<TerrainShader> shader);

	void render(const std::vector<Terrain>& terrains);
};
