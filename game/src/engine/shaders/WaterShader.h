#pragma once
#include <unordered_map>

#include "Shader.h"

class Water;
class Environment;

class WaterShader final: public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ViewMatrix, ProjectionMatrix, CameraPosition,
		Reflection, Refraction, DistortionMap, NormalMap, DepthMap, MoveFactor,
		LightColor, LightPosition,
		NearPlane, FarPlane
	};

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

	static constexpr char vertex_file[] = "water-vert";
	static constexpr char fragment_file[] = "water-frag";

	void bindAttributes() override;
	void getAllUniformLocations() override;
	
public:
	void setUp() override;
	void loadModelMatrix(const Water& water);
	void loadUniformPerFrame(const Environment& environment);
	void connectTextureUnits();
};
