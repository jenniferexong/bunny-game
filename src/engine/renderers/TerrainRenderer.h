#pragma once

#include <memory>
#include <vector>

class TerrainShader;
class Terrain;

class TerrainRenderer {
private:
	static constexpr char name_[] = "TerrainRenderer";

	std::shared_ptr<TerrainShader> shader_;

	void prepareTerrain(const Terrain& terrain);
	void unbindTerrain();
	void bindTextures(const Terrain& terrain);
	void loadTransformation(const Terrain& terrain);

public:
	TerrainRenderer(): shader_(nullptr) {}
	TerrainRenderer(std::shared_ptr<TerrainShader> shader);
	~TerrainRenderer() = default;

	void render(const std::vector<Terrain>& terrains);
};
