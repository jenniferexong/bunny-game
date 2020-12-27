#pragma once

#include <memory>

#include "../ShadowBox.h"

#include "../post-processing/Fbo.h"
#include "../ShadowFrameBuffer.h"
#include "../shaders/ShadowShader.h"

class Mesh;
class Entity;
class Environment;

class ShadowRenderer {
private:
	static const int shadow_map_size;
	// orthographic projection
	glm::mat4 projection_matrix_; 
	// sun's view 
	glm::mat4 light_view_matrix_ = glm::mat4(1.f);

	ShadowShader shader_;
	ShadowBox shadow_box_;
	// TODO:
	/*
	ShadowFrameBuffer fbo_ = ShadowFrameBuffer(
		shadow_map_size,
		shadow_map_size
	);
	*/
	Fbo fbo_ = Fbo(
		shadow_map_size,
		shadow_map_size,
		DepthBufferAttachment::DepthTexture
	);

	void prepare(vec3 light_direction, const Camera& camera);
	void updateOrthoProjection(float width, float height, float length);
	void updateLightView(vec3 direction, vec3 center);
	void finish();

	void bindMesh(const Mesh& mesh);
	void prepareInstance(const Entity& entity);
	static glm::mat4 createOffset();

public:
	ShadowRenderer();
	void render(const Environment& environment);
	void renderEntities(const Environment& environment);
	void renderTerrain(const Environment& environment);

	int getShadowMapTexture() const 
	{ 
		return fbo_.getDepthTexture();
		//return fbo_.getShadowMap(); 
	}
	glm::mat4 getLightSpaceMatrix();
};
