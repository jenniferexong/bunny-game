
#include <gl/glew.h>

#include "SkyboxRenderer.h"

#include "../Application.h"
#include "../Location.h"

const float SkyboxRenderer::cube_size = 500.f;

SkyboxRenderer::SkyboxRenderer()
{
	printf("create skybox renderer\n");
	cube_ = Application::loader.loadToVao(vertex_positions, 3);
	day_texture_id_ = Application::loader.loadCubeMap(sky_texture_names);
	night_texture_id_ = Application::loader.loadCubeMap(star_texture_names);
	shader_.setUp();

	shader_.start();
	shader_.connectTextureUnits();
	shader_.stop();
}

// order of textures for faces must be: right, left, top, bottom, back, front
const std::vector<std::string> SkyboxRenderer::sky_texture_names = {
	"side", "side", "top", "bottom", "side", "side"
};

const std::vector<std::string> SkyboxRenderer::star_texture_names = {
	"stars-right", "stars-left", "stars-top", "stars-bottom", "stars-back", "stars-front"
};


void SkyboxRenderer::render()
{
	shader_.start();

	glBindVertexArray(cube_.getId());
	glEnableVertexAttribArray(AttributeLocation::Position);

	bindTextures();

	shader_.loadUniforms();

	glDrawArrays(GL_TRIANGLES, 0, cube_.getVertexCount());

	glDisableVertexAttribArray(AttributeLocation::Position);
	glBindVertexArray(0);

	shader_.stop();
}

void SkyboxRenderer::bindTextures()
{
	// TODO: make sunlight change with day and night, fix fog?
	time += Application::frame_delta * 1000;
	time = fmod(time, 24000);
	int texture1;
	int texture2;
	float blend_factor;
	if (time >= 0 && time < 5000) {
		texture1 = night_texture_id_;
		texture2 = night_texture_id_;
		blend_factor = (time - 0) / (5000 - 0);
	}
	else if (time >= 5000 && time < 8000) {
		texture1 = night_texture_id_;
		texture2 = day_texture_id_;
		blend_factor = (time - 5000) / (8000 - 5000);
	}
	else if (time >= 8000 && time < 21000) {
		texture1 = day_texture_id_;
		texture2 = day_texture_id_;
		blend_factor = (time - 8000) / (21000 - 8000);
	}
	else {
		texture1 = day_texture_id_;
		texture2 = night_texture_id_;
		blend_factor = (time - 21000) / (24000 - 21000);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
	shader_.loadBlendFactor(blend_factor);
}

const std::vector<float> SkyboxRenderer::vertex_positions = {
		-cube_size,  cube_size, -cube_size,
		-cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,
		 cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,

		-cube_size, -cube_size,  cube_size,
		-cube_size, -cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size,  cube_size,
		-cube_size, -cube_size,  cube_size,

		 cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,

		-cube_size, -cube_size,  cube_size,
		-cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size, -cube_size,  cube_size,
		-cube_size, -cube_size,  cube_size,

		-cube_size,  cube_size, -cube_size,
		 cube_size,  cube_size, -cube_size,
		 cube_size,  cube_size,  cube_size,
		 cube_size,  cube_size,  cube_size,
		-cube_size,  cube_size,  cube_size,
		-cube_size,  cube_size, -cube_size,

		-cube_size, -cube_size, -cube_size,
		-cube_size, -cube_size,  cube_size,
		 cube_size, -cube_size, -cube_size,
		 cube_size, -cube_size, -cube_size,
		-cube_size, -cube_size,  cube_size,
		 cube_size, -cube_size,  cube_size
};
