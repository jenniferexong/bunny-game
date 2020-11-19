#pragma once
#include "Shader.h"

class WaterShader final: public Shader {
private:
	enum class UniformVariable {
		TransformationMatrix, ViewMatrix, ProjectionMatrix, CameraPosition,
		Reflection, Refraction, DistortionMap, NormalMap, DepthMap, MoveFactor,
		LightColor, LightPosition,
		NearPlane, FarPlane
	};

	std::unordered_map<UniformVariable, int> locations_;

	static const std::string vertex_file;
	static const std::string fragment_file;

	void bindAttributes() override;
	void getAllUniformLocations() override;
	
public:
	void setUp() override;
	void loadModelMatrix(const Water& water);
	void loadUniformPerFrame(const Environment& environment, float move_factor);
	void connectTextureUnits();
};
