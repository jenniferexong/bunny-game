#pragma once

#include <memory>
#include <vector>

#include "../shaders/TerrainShader.h"
#include "../objects/Terrain.h"

class TerrainRenderer {
private:
	std::shared_ptr<TerrainShader> m_shader;
public:
	TerrainRenderer(): m_shader(nullptr) {}
	TerrainRenderer(std::shared_ptr<TerrainShader> shader) : m_shader(shader) {}

	void render(std::vector<);
};
