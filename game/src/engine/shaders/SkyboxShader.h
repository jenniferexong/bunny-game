#pragma once

#include <unordered_map>

#include "Shader.h"

class Camera;

class SkyboxShader final: public Shader {
	
private:
	enum class UniformVariable {
		ProjectionMatrix, ViewMatrix, 
		DayTexture, NightTexture, BlendFactor
	};

	static constexpr char vertex_file[] = "skybox-vert";
	static constexpr char fragment_file[] = "skybox-frag";

	static constexpr float rotate_speed = 0.1f;
	float current_rotation_ = 0.f;

	std::unordered_map<UniformVariable, int, EnumHash> locations_;

protected:
	void bindAttributes() override;
	void getAllUniformLocations() override;

public:
	void setUp() override;
	void loadUniforms(const Camera& camera);
	void connectTextureUnits();
	void loadBlendFactor(float blend);
};
